/*
 * OpenR2 
 * MFC/R2 call setup library
 *
 * r2engine.h - MFC/R2 tone generation and detection.
 *              DTMF tone generation and detection.
 *
 * Borrowed and slightly modified from the LGPL SpanDSP library, 
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001 Steve Underwood
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include "openr2/r2utils-pvt.h"
#include "openr2/r2engine.h"

#if !defined(M_PI)
/* C99 systems may not define M_PI */
#define M_PI 3.14159265358979323846264338327
#endif

#define ms_to_samples(t)            (((t)*SAMPLE_RATE)/1000)

#define FALSE 0
#define TRUE (!FALSE)
#define SAMPLE_RATE 8000
#define DBM0_MAX_SINE_POWER     (3.14f)

/* Direct Digital Synthesis required for tone generation */
static int32_t dds_phase_ratef(float frequency);
static float dds_scaling_dbm0f(float level);
static float dds_modf(uint32_t *phase_acc, int32_t phase_rate, float scale, int32_t phase);

/* generic tone generation */
static void make_tone_gen_descriptor(openr2_tone_gen_descriptor_t *s, int f1, int l1, int f2, int l2, int d1, int d2, int d3, int d4, int repeat);
static openr2_tone_gen_state_t *tone_gen_init(openr2_tone_gen_state_t *s, openr2_tone_gen_descriptor_t *t);
static int tone_gen(openr2_tone_gen_state_t *s, int16_t amp[], int max_samples);

/* Goertzel Algorithm for tone detection */
static void make_goertzel_descriptor(openr2_goertzel_descriptor_t *t, float freq, int samples);
static openr2_goertzel_state_t *goertzel_init(openr2_goertzel_state_t *s, openr2_goertzel_descriptor_t *t);
static void goertzel_reset(openr2_goertzel_state_t *s);
static float goertzel_result(openr2_goertzel_state_t *s);

typedef struct
{
    float       f1;         /* First freq */
    float       f2;         /* Second freq */
    int8_t      level1;     /* Level of the first freq (dB) */
    int8_t      level2;     /* Level of the second freq (dB) */
    uint8_t     on_time;    /* Tone on time (ms) */
    uint8_t     off_time;   /* Minimum post tone silence (ms) */
} mf_digit_tones_t;

static int r2_mf_gen_inited = FALSE;
static openr2_tone_gen_descriptor_t r2_mf_fwd_digit_tones[15];
static openr2_tone_gen_descriptor_t r2_mf_back_digit_tones[15];

/* R2 tone generation specs.
 *  Power: -11.5dBm +- 1dB
 *  Frequency: within +-4Hz
 *  Mismatch between the start time of a pair of tones: <=1ms.
 *  Mismatch between the end time of a pair of tones: <=1ms.
 */
static const mf_digit_tones_t r2_mf_fwd_tones[] =
{
    {1380.0f, 1500.0f, -11, -11, 1, 0},
    {1380.0f, 1620.0f, -11, -11, 1, 0},
    {1500.0f, 1620.0f, -11, -11, 1, 0},
    {1380.0f, 1740.0f, -11, -11, 1, 0},
    {1500.0f, 1740.0f, -11, -11, 1, 0},
    {1620.0f, 1740.0f, -11, -11, 1, 0},
    {1380.0f, 1860.0f, -11, -11, 1, 0},
    {1500.0f, 1860.0f, -11, -11, 1, 0},
    {1620.0f, 1860.0f, -11, -11, 1, 0},
    {1740.0f, 1860.0f, -11, -11, 1, 0},
    {1380.0f, 1980.0f, -11, -11, 1, 0},
    {1500.0f, 1980.0f, -11, -11, 1, 0},
    {1620.0f, 1980.0f, -11, -11, 1, 0},
    {1740.0f, 1980.0f, -11, -11, 1, 0},
    {1860.0f, 1980.0f, -11, -11, 1, 0},
    {0.0f, 0.0f, 0, 0, 0, 0}
};

static const mf_digit_tones_t r2_mf_back_tones[] =
{
    {1140.0f, 1020.0f, -11, -11, 1, 0},
    {1140.0f,  900.0f, -11, -11, 1, 0},
    {1020.0f,  900.0f, -11, -11, 1, 0},
    {1140.0f,  780.0f, -11, -11, 1, 0},
    {1020.0f,  780.0f, -11, -11, 1, 0},
    { 900.0f,  780.0f, -11, -11, 1, 0},
    {1140.0f,  660.0f, -11, -11, 1, 0},
    {1020.0f,  660.0f, -11, -11, 1, 0},
    { 900.0f,  660.0f, -11, -11, 1, 0},
    { 780.0f,  660.0f, -11, -11, 1, 0},
    {1140.0f,  540.0f, -11, -11, 1, 0},
    {1020.0f,  540.0f, -11, -11, 1, 0},
    { 900.0f,  540.0f, -11, -11, 1, 0},
    { 780.0f,  540.0f, -11, -11, 1, 0},
    { 660.0f,  540.0f, -11, -11, 1, 0},
    {0.0f, 0.0f, 0, 0, 0, 0}
};

/* The order of the digits here must match the lists above */
static const char r2_mf_tone_codes[] = "1234567890BCDEF";

#define R2_MF_THRESHOLD             5.0e8f
#define R2_MF_TWIST                 5.0f    /* 7dB */
#define R2_MF_RELATIVE_PEAK         12.6f   /* 11dB */
#define R2_MF_SAMPLES_PER_BLOCK     133

static openr2_goertzel_descriptor_t mf_fwd_detect_desc[6];
static openr2_goertzel_descriptor_t mf_back_detect_desc[6];

static const float r2_mf_fwd_frequencies[] =
{
    1380.0f, 1500.0f, 1620.0f, 1740.0f, 1860.0f, 1980.0f
};

static const float r2_mf_back_frequencies[] =
{
    1140.0f, 1020.0f,  900.0f,  780.0f,  660.0f,  540.0f
};

/* Use codes '1' to 'F' for the R2 signals 1 to 15, except for signal 'A'.
   Use '0' for this, so the codes match the digits 0-9. */
static const char r2_mf_positions[] = "1247B-358C--69D---0E----F";

int openr2_mf_tx(openr2_mf_tx_state_t *s, int16_t amp[], int samples)
{
    int len;

    if (s->digit == 0)
    {
        len = samples;
        memset(amp, 0, len*sizeof(int16_t));
    }
    else
    {
        len = tone_gen(&s->tone, amp, samples);
    }
    return len;
}

int openr2_mf_tx_put(openr2_mf_tx_state_t *s, char digit)
{
    char *cp;

    if (digit  &&  (cp = strchr(r2_mf_tone_codes, digit)))
    {
        if (s->fwd)
            tone_gen_init(&s->tone, &r2_mf_fwd_digit_tones[cp - r2_mf_tone_codes]);
        else
            tone_gen_init(&s->tone, &r2_mf_back_digit_tones[cp - r2_mf_tone_codes]);
        s->digit = digit;
    }
    else
    {
        s->digit = 0;
    }
    return 0;
}

openr2_mf_tx_state_t *openr2_mf_tx_init(openr2_mf_tx_state_t *s, int fwd)
{
    int i;
    const mf_digit_tones_t *tones;

    if (s == NULL)
    {
        if ((s = (openr2_mf_tx_state_t *) malloc(sizeof(*s))) == NULL)
            return NULL;
    }
    memset(s, 0, sizeof(*s));

    if (!r2_mf_gen_inited)
    {
        i = 0;
        tones = r2_mf_fwd_tones;
        while (tones->on_time)
        {
            make_tone_gen_descriptor(&r2_mf_fwd_digit_tones[i++],
                                     (int) tones->f1,
                                     tones->level1,
                                     (int) tones->f2,
                                     tones->level2,
                                     tones->on_time,
                                     tones->off_time,
                                     0,
                                     0,
                                     (tones->off_time == 0));
            tones++;
        }
        i = 0;
        tones = r2_mf_back_tones;
        while (tones->on_time)
        {
            make_tone_gen_descriptor(&r2_mf_back_digit_tones[i++],
                                     (int) tones->f1,
                                     tones->level1,
                                     (int) tones->f2,
                                     tones->level2,
                                     tones->on_time,
                                     tones->off_time,
                                     0,
                                     0,
                                     (tones->off_time == 0));
            tones++;
        }
        r2_mf_gen_inited = TRUE;
    }
    s->fwd = fwd;
    return s;
}

int openr2_mf_rx(openr2_mf_rx_state_t *s, const int16_t amp[], int samples)
{
    float energy[6];
    float famp;
    float v1;
    int i;
    int j;
    int sample;
    int best;
    int second_best;
    int hit;
    int hit_digit;
    int limit;

    hit = 0;
    hit_digit = 0;
    for (sample = 0;  sample < samples;  sample = limit)
    {
        if ((samples - sample) >= (R2_MF_SAMPLES_PER_BLOCK - s->current_sample))
            limit = sample + (R2_MF_SAMPLES_PER_BLOCK - s->current_sample);
        else
            limit = samples;
        for (j = sample;  j < limit;  j++)
        {
            famp = amp[j];
            /* With GCC 2.95, the following unrolled code seems to take about 35%
               (rough estimate) as long as a neat little 0-5 loop */
            v1 = s->out[0].v2;
            s->out[0].v2 = s->out[0].v3;
            s->out[0].v3 = s->out[0].fac*s->out[0].v2 - v1 + famp;
    
            v1 = s->out[1].v2;
            s->out[1].v2 = s->out[1].v3;
            s->out[1].v3 = s->out[1].fac*s->out[1].v2 - v1 + famp;
    
            v1 = s->out[2].v2;
            s->out[2].v2 = s->out[2].v3;
            s->out[2].v3 = s->out[2].fac*s->out[2].v2 - v1 + famp;
    
            v1 = s->out[3].v2;
            s->out[3].v2 = s->out[3].v3;
            s->out[3].v3 = s->out[3].fac*s->out[3].v2 - v1 + famp;
    
            v1 = s->out[4].v2;
            s->out[4].v2 = s->out[4].v3;
            s->out[4].v3 = s->out[4].fac*s->out[4].v2 - v1 + famp;
    
            v1 = s->out[5].v2;
            s->out[5].v2 = s->out[5].v3;
            s->out[5].v3 = s->out[5].fac*s->out[5].v2 - v1 + famp;
        }
        s->current_sample += (limit - sample);
        if (s->current_sample < R2_MF_SAMPLES_PER_BLOCK)
            continue;

        /* We are at the end of an MF detection block */
        /* Find the two highest energies */
        energy[0] = goertzel_result(&s->out[0]);
        energy[1] = goertzel_result(&s->out[1]);
        if (energy[0] > energy[1])
        {
            best = 0;
            second_best = 1;
        }
        else
        {
            best = 1;
            second_best = 0;
        }
        
        for (i = 2;  i < 6;  i++)
        {
            energy[i] = goertzel_result(&s->out[i]);
            if (energy[i] >= energy[best])
            {
                second_best = best;
                best = i;
            }
            else if (energy[i] >= energy[second_best])
            {
                second_best = i;
            }
        }
        /* Basic signal level and twist tests */
        hit = FALSE;
        if (energy[best] >= R2_MF_THRESHOLD
            &&
            energy[second_best] >= R2_MF_THRESHOLD
            &&
            energy[best] < energy[second_best]*R2_MF_TWIST
            &&
            energy[best]*R2_MF_TWIST > energy[second_best])
        {
            /* Relative peak test */
            hit = TRUE;
            for (i = 0;  i < 6;  i++)
            {
                if (i != best  &&  i != second_best)
                {
                    if (energy[i]*R2_MF_RELATIVE_PEAK >= energy[second_best])
                    {
                        /* The best two are not clearly the best */
                        hit = FALSE;
                        break;
                    }
                }
            }
        }
        if (hit)
        {
            /* Get the values into ascending order */
            if (second_best < best)
            {
                i = best;
                best = second_best;
                second_best = i;
            }
            best = best*5 + second_best - 1;
            hit_digit = r2_mf_positions[best];
        }
        else
        {
            hit_digit = 0;
        }
        s->current_digit = hit_digit;

        /* Reinitialise the detector for the next block */
        for (i = 0;  i < 6;  i++)
            goertzel_reset(&s->out[i]);
        s->current_sample = 0;
    }
    return hit_digit;
}

openr2_mf_rx_state_t *openr2_mf_rx_init(openr2_mf_rx_state_t *s, int fwd)
{
    int i;
    static int initialised = FALSE;

    if (s == NULL)
    {
        if ((s = (openr2_mf_rx_state_t *) malloc(sizeof(*s))) == NULL)
            return NULL;
    }
    memset(s, 0, sizeof(*s));

    s->fwd = fwd;

    if (!initialised)
    {
        for (i = 0;  i < 6;  i++)
        {
            make_goertzel_descriptor(&mf_fwd_detect_desc[i], r2_mf_fwd_frequencies[i], R2_MF_SAMPLES_PER_BLOCK);
            make_goertzel_descriptor(&mf_back_detect_desc[i], r2_mf_back_frequencies[i], R2_MF_SAMPLES_PER_BLOCK);
        }
        initialised = TRUE;
    }
    if (fwd)
    {
        for (i = 0;  i < 6;  i++)
            goertzel_init(&s->out[i], &mf_fwd_detect_desc[i]);
    }
    else
    {
        for (i = 0;  i < 6;  i++)
            goertzel_init(&s->out[i], &mf_back_detect_desc[i]);
    }
    s->current_digit = 0;
    s->current_sample = 0;
    return s;
}

static void make_goertzel_descriptor(openr2_goertzel_descriptor_t *t, float freq, int samples)
{
    t->fac = 2.0f*cosf(2.0f*M_PI*(freq/(float) SAMPLE_RATE));
    t->samples = samples;
}

static openr2_goertzel_state_t *goertzel_init(openr2_goertzel_state_t *s, openr2_goertzel_descriptor_t *t)
{
    if (s == NULL)
    {
        if ((s = (openr2_goertzel_state_t *) malloc(sizeof(*s))) == NULL)
            return NULL;
    }
    s->v2 =
    s->v3 = 0.0;
    s->fac = t->fac;
    s->samples = t->samples;
    s->current_sample = 0;
    return s;
}

static void goertzel_reset(openr2_goertzel_state_t *s)
{
    s->v2 =
    s->v3 = 0.0;
    s->current_sample = 0;
}

static float goertzel_result(openr2_goertzel_state_t *s)
{
    float v1;
    /* Push a zero through the process to finish things off. */
    v1 = s->v2;
    s->v2 = s->v3;
    s->v3 = s->fac*s->v2 - v1;
    /* Now calculate the non-recursive side of the filter. */
    /* The result here is not scaled down to allow for the magnification
       effect of the filter (the usual DFT magnification effect). */
    return s->v3*s->v3 + s->v2*s->v2 - s->v2*s->v3*s->fac;
}

static void make_tone_gen_descriptor(openr2_tone_gen_descriptor_t *s,
                              int f1,
                              int l1,
                              int f2,
                              int l2,
                              int d1,
                              int d2,
                              int d3,
                              int d4,
                              int repeat)
{
    memset(s, 0, sizeof(*s));
    if (f1)
    {
        s->tone[0].phase_rate = dds_phase_ratef((float) f1);
        if (f2 < 0)
            s->tone[0].phase_rate = -s->tone[0].phase_rate;
        s->tone[0].gain = dds_scaling_dbm0f((float) l1);
    }
    if (f2)
    {
        s->tone[1].phase_rate = dds_phase_ratef((float) abs(f2));
        s->tone[1].gain = (f2 < 0)  ?  (float) l2/100.0f  :  dds_scaling_dbm0f((float) l2);
    }

    s->duration[0] = d1*SAMPLE_RATE/1000;
    s->duration[1] = d2*SAMPLE_RATE/1000;
    s->duration[2] = d3*SAMPLE_RATE/1000;
    s->duration[3] = d4*SAMPLE_RATE/1000;

    s->repeat = repeat;
}

static openr2_tone_gen_state_t *tone_gen_init(openr2_tone_gen_state_t *s, openr2_tone_gen_descriptor_t *t)
{
    int i;

    if (s == NULL)
        return NULL;
    for (i = 0;  i < 4;  i++)
    {
        s->tone[i] = t->tone[i];
        s->phase[i] = 0;
    }

    for (i = 0;  i < 4;  i++)
        s->duration[i] = t->duration[i];
    s->repeat = t->repeat;

    s->current_section = 0;
    s->current_position = 0;
    return s;
}

static int tone_gen(openr2_tone_gen_state_t *s, int16_t amp[], int max_samples)
{
    int samples;
    int limit;
    float xamp;
    int i;

    if (s->current_section < 0)
        return  0;

    for (samples = 0;  samples < max_samples;  )
    {
        limit = samples + s->duration[s->current_section] - s->current_position;
        if (limit > max_samples)
            limit = max_samples;
        
        s->current_position += (limit - samples);
        if (s->current_section & 1)
        {
            /* A silent section */
            for (  ;  samples < limit;  samples++)
                amp[samples] = 0;
        }
        else
        {
            if (s->tone[0].phase_rate < 0)
            {
                for (  ;  samples < limit;  samples++)
                {
                    /* There must be two, and only two tones */
                    xamp = dds_modf(&s->phase[0], -s->tone[0].phase_rate, s->tone[0].gain, 0)
                         *(1.0f + dds_modf(&s->phase[1], s->tone[1].phase_rate, s->tone[1].gain, 0));
                    amp[samples] = (int16_t) rintf(xamp);
                }
            }
            else
            {
                for (  ;  samples < limit;  samples++)
                {
                    xamp = 0.0f;
                    for (i = 0;  i < 4;  i++)
                    {
                        if (s->tone[i].phase_rate == 0)
                            break;
                        xamp += dds_modf(&s->phase[i], s->tone[i].phase_rate, s->tone[i].gain, 0);
                    }
                    /* Saturation of the answer is the right thing at this point.
                       However, we are normally generating well controlled tones,
                       that cannot clip. So, the overhead of doing saturation is
                       a waste of valuable time. */
                    amp[samples] = (int16_t) rintf(xamp);
                }
            }
        }
        if (s->current_position >= s->duration[s->current_section])
        {
            s->current_position = 0;
            if (++s->current_section > 3  ||  s->duration[s->current_section] == 0)
            {
                if (!s->repeat)
                {
                    /* Force a quick exit */
                    s->current_section = -1;
                    break;
                }
                s->current_section = 0;
            }
        }
    }
    return samples;
}

#define SLENK       11
#define SINELEN     (1 << SLENK)

/* Precreating this table allows it to be in const memory, which might
   have some performance advantage. */
static const float sine_table[SINELEN] =
{
     0.00000000f,
     0.00306796f,
     0.00613588f,
     0.00920375f,
     0.01227154f,
     0.01533921f,
     0.01840673f,
     0.02147408f,
     0.02454123f,
     0.02760815f,
     0.03067480f,
     0.03374117f,
     0.03680722f,
     0.03987293f,
     0.04293826f,
     0.04600318f,
     0.04906767f,
     0.05213170f,
     0.05519524f,
     0.05825826f,
     0.06132074f,
     0.06438263f,
     0.06744392f,
     0.07050457f,
     0.07356456f,
     0.07662386f,
     0.07968244f,
     0.08274026f,
     0.08579731f,
     0.08885355f,
     0.09190896f,
     0.09496350f,
     0.09801714f,
     0.10106986f,
     0.10412163f,
     0.10717242f,
     0.11022221f,
     0.11327095f,
     0.11631863f,
     0.11936521f,
     0.12241068f,
     0.12545498f,
     0.12849811f,
     0.13154003f,
     0.13458071f,
     0.13762012f,
     0.14065824f,
     0.14369503f,
     0.14673047f,
     0.14976453f,
     0.15279719f,
     0.15582840f,
     0.15885814f,
     0.16188639f,
     0.16491312f,
     0.16793829f,
     0.17096189f,
     0.17398387f,
     0.17700422f,
     0.18002290f,
     0.18303989f,
     0.18605515f,
     0.18906866f,
     0.19208040f,
     0.19509032f,
     0.19809841f,
     0.20110463f,
     0.20410897f,
     0.20711138f,
     0.21011184f,
     0.21311032f,
     0.21610680f,
     0.21910124f,
     0.22209362f,
     0.22508391f,
     0.22807208f,
     0.23105811f,
     0.23404196f,
     0.23702361f,
     0.24000302f,
     0.24298018f,
     0.24595505f,
     0.24892761f,
     0.25189782f,
     0.25486566f,
     0.25783110f,
     0.26079412f,
     0.26375468f,
     0.26671276f,
     0.26966833f,
     0.27262136f,
     0.27557182f,
     0.27851969f,
     0.28146494f,
     0.28440754f,
     0.28734746f,
     0.29028468f,
     0.29321916f,
     0.29615089f,
     0.29907983f,
     0.30200595f,
     0.30492923f,
     0.30784964f,
     0.31076715f,
     0.31368174f,
     0.31659338f,
     0.31950203f,
     0.32240768f,
     0.32531029f,
     0.32820984f,
     0.33110631f,
     0.33399965f,
     0.33688985f,
     0.33977688f,
     0.34266072f,
     0.34554132f,
     0.34841868f,
     0.35129276f,
     0.35416353f,
     0.35703096f,
     0.35989504f,
     0.36275572f,
     0.36561300f,
     0.36846683f,
     0.37131719f,
     0.37416406f,
     0.37700741f,
     0.37984721f,
     0.38268343f,
     0.38551605f,
     0.38834505f,
     0.39117038f,
     0.39399204f,
     0.39680999f,
     0.39962420f,
     0.40243465f,
     0.40524131f,
     0.40804416f,
     0.41084317f,
     0.41363831f,
     0.41642956f,
     0.41921689f,
     0.42200027f,
     0.42477968f,
     0.42755509f,
     0.43032648f,
     0.43309382f,
     0.43585708f,
     0.43861624f,
     0.44137127f,
     0.44412214f,
     0.44686884f,
     0.44961133f,
     0.45234959f,
     0.45508359f,
     0.45781330f,
     0.46053871f,
     0.46325978f,
     0.46597650f,
     0.46868882f,
     0.47139674f,
     0.47410021f,
     0.47679923f,
     0.47949376f,
     0.48218377f,
     0.48486925f,
     0.48755016f,
     0.49022648f,
     0.49289819f,
     0.49556526f,
     0.49822767f,
     0.50088538f,
     0.50353838f,
     0.50618665f,
     0.50883014f,
     0.51146885f,
     0.51410274f,
     0.51673180f,
     0.51935599f,
     0.52197529f,
     0.52458968f,
     0.52719913f,
     0.52980362f,
     0.53240313f,
     0.53499762f,
     0.53758708f,
     0.54017147f,
     0.54275078f,
     0.54532499f,
     0.54789406f,
     0.55045797f,
     0.55301671f,
     0.55557023f,
     0.55811853f,
     0.56066158f,
     0.56319934f,
     0.56573181f,
     0.56825895f,
     0.57078075f,
     0.57329717f,
     0.57580819f,
     0.57831380f,
     0.58081396f,
     0.58330865f,
     0.58579786f,
     0.58828155f,
     0.59075970f,
     0.59323230f,
     0.59569930f,
     0.59816071f,
     0.60061648f,
     0.60306660f,
     0.60551104f,
     0.60794978f,
     0.61038281f,
     0.61281008f,
     0.61523159f,
     0.61764731f,
     0.62005721f,
     0.62246128f,
     0.62485949f,
     0.62725182f,
     0.62963824f,
     0.63201874f,
     0.63439328f,
     0.63676186f,
     0.63912444f,
     0.64148101f,
     0.64383154f,
     0.64617601f,
     0.64851440f,
     0.65084668f,
     0.65317284f,
     0.65549285f,
     0.65780669f,
     0.66011434f,
     0.66241578f,
     0.66471098f,
     0.66699992f,
     0.66928259f,
     0.67155895f,
     0.67382900f,
     0.67609270f,
     0.67835004f,
     0.68060100f,
     0.68284555f,
     0.68508367f,
     0.68731534f,
     0.68954054f,
     0.69175926f,
     0.69397146f,
     0.69617713f,
     0.69837625f,
     0.70056879f,
     0.70275474f,
     0.70493408f,
     0.70710678f,
     0.70927283f,
     0.71143220f,
     0.71358487f,
     0.71573083f,
     0.71787005f,
     0.72000251f,
     0.72212819f,
     0.72424708f,
     0.72635916f,
     0.72846439f,
     0.73056277f,
     0.73265427f,
     0.73473888f,
     0.73681657f,
     0.73888732f,
     0.74095113f,
     0.74300795f,
     0.74505779f,
     0.74710061f,
     0.74913639f,
     0.75116513f,
     0.75318680f,
     0.75520138f,
     0.75720885f,
     0.75920919f,
     0.76120239f,
     0.76318842f,
     0.76516727f,
     0.76713891f,
     0.76910334f,
     0.77106052f,
     0.77301045f,
     0.77495311f,
     0.77688847f,
     0.77881651f,
     0.78073723f,
     0.78265060f,
     0.78455660f,
     0.78645521f,
     0.78834643f,
     0.79023022f,
     0.79210658f,
     0.79397548f,
     0.79583690f,
     0.79769084f,
     0.79953727f,
     0.80137617f,
     0.80320753f,
     0.80503133f,
     0.80684755f,
     0.80865618f,
     0.81045720f,
     0.81225059f,
     0.81403633f,
     0.81581441f,
     0.81758481f,
     0.81934752f,
     0.82110251f,
     0.82284978f,
     0.82458930f,
     0.82632106f,
     0.82804505f,
     0.82976123f,
     0.83146961f,
     0.83317016f,
     0.83486287f,
     0.83654773f,
     0.83822471f,
     0.83989379f,
     0.84155498f,
     0.84320824f,
     0.84485357f,
     0.84649094f,
     0.84812034f,
     0.84974177f,
     0.85135519f,
     0.85296060f,
     0.85455799f,
     0.85614733f,
     0.85772861f,
     0.85930182f,
     0.86086694f,
     0.86242396f,
     0.86397286f,
     0.86551362f,
     0.86704625f,
     0.86857071f,
     0.87008699f,
     0.87159509f,
     0.87309498f,
     0.87458665f,
     0.87607009f,
     0.87754529f,
     0.87901223f,
     0.88047089f,
     0.88192126f,
     0.88336334f,
     0.88479710f,
     0.88622253f,
     0.88763962f,
     0.88904836f,
     0.89044872f,
     0.89184071f,
     0.89322430f,
     0.89459949f,
     0.89596625f,
     0.89732458f,
     0.89867447f,
     0.90001589f,
     0.90134885f,
     0.90267332f,
     0.90398929f,
     0.90529676f,
     0.90659570f,
     0.90788612f,
     0.90916798f,
     0.91044129f,
     0.91170603f,
     0.91296219f,
     0.91420976f,
     0.91544872f,
     0.91667906f,
     0.91790078f,
     0.91911385f,
     0.92031828f,
     0.92151404f,
     0.92270113f,
     0.92387953f,
     0.92504924f,
     0.92621024f,
     0.92736253f,
     0.92850608f,
     0.92964090f,
     0.93076696f,
     0.93188427f,
     0.93299280f,
     0.93409255f,
     0.93518351f,
     0.93626567f,
     0.93733901f,
     0.93840353f,
     0.93945922f,
     0.94050607f,
     0.94154407f,
     0.94257320f,
     0.94359346f,
     0.94460484f,
     0.94560733f,
     0.94660091f,
     0.94758559f,
     0.94856135f,
     0.94952818f,
     0.95048607f,
     0.95143502f,
     0.95237501f,
     0.95330604f,
     0.95422810f,
     0.95514117f,
     0.95604525f,
     0.95694034f,
     0.95782641f,
     0.95870347f,
     0.95957151f,
     0.96043052f,
     0.96128049f,
     0.96212140f,
     0.96295327f,
     0.96377607f,
     0.96458979f,
     0.96539444f,
     0.96619000f,
     0.96697647f,
     0.96775384f,
     0.96852209f,
     0.96928124f,
     0.97003125f,
     0.97077214f,
     0.97150389f,
     0.97222650f,
     0.97293995f,
     0.97364425f,
     0.97433938f,
     0.97502535f,
     0.97570213f,
     0.97636973f,
     0.97702814f,
     0.97767736f,
     0.97831737f,
     0.97894818f,
     0.97956977f,
     0.98018214f,
     0.98078528f,
     0.98137919f,
     0.98196387f,
     0.98253930f,
     0.98310549f,
     0.98366242f,
     0.98421009f,
     0.98474850f,
     0.98527764f,
     0.98579751f,
     0.98630810f,
     0.98680940f,
     0.98730142f,
     0.98778414f,
     0.98825757f,
     0.98872169f,
     0.98917651f,
     0.98962202f,
     0.99005821f,
     0.99048508f,
     0.99090264f,
     0.99131086f,
     0.99170975f,
     0.99209931f,
     0.99247953f,
     0.99285041f,
     0.99321195f,
     0.99356414f,
     0.99390697f,
     0.99424045f,
     0.99456457f,
     0.99487933f,
     0.99518473f,
     0.99548076f,
     0.99576741f,
     0.99604470f,
     0.99631261f,
     0.99657115f,
     0.99682030f,
     0.99706007f,
     0.99729046f,
     0.99751146f,
     0.99772307f,
     0.99792529f,
     0.99811811f,
     0.99830154f,
     0.99847558f,
     0.99864022f,
     0.99879546f,
     0.99894129f,
     0.99907773f,
     0.99920476f,
     0.99932238f,
     0.99943060f,
     0.99952942f,
     0.99961882f,
     0.99969882f,
     0.99976941f,
     0.99983058f,
     0.99988235f,
     0.99992470f,
     0.99995764f,
     0.99998118f,
     0.99999529f,
     1.00000000f,
     0.99999529f,
     0.99998118f,
     0.99995764f,
     0.99992470f,
     0.99988235f,
     0.99983058f,
     0.99976941f,
     0.99969882f,
     0.99961882f,
     0.99952942f,
     0.99943060f,
     0.99932238f,
     0.99920476f,
     0.99907773f,
     0.99894129f,
     0.99879546f,
     0.99864022f,
     0.99847558f,
     0.99830154f,
     0.99811811f,
     0.99792529f,
     0.99772307f,
     0.99751146f,
     0.99729046f,
     0.99706007f,
     0.99682030f,
     0.99657115f,
     0.99631261f,
     0.99604470f,
     0.99576741f,
     0.99548076f,
     0.99518473f,
     0.99487933f,
     0.99456457f,
     0.99424045f,
     0.99390697f,
     0.99356414f,
     0.99321195f,
     0.99285041f,
     0.99247953f,
     0.99209931f,
     0.99170975f,
     0.99131086f,
     0.99090264f,
     0.99048508f,
     0.99005821f,
     0.98962202f,
     0.98917651f,
     0.98872169f,
     0.98825757f,
     0.98778414f,
     0.98730142f,
     0.98680940f,
     0.98630810f,
     0.98579751f,
     0.98527764f,
     0.98474850f,
     0.98421009f,
     0.98366242f,
     0.98310549f,
     0.98253930f,
     0.98196387f,
     0.98137919f,
     0.98078528f,
     0.98018214f,
     0.97956977f,
     0.97894818f,
     0.97831737f,
     0.97767736f,
     0.97702814f,
     0.97636973f,
     0.97570213f,
     0.97502535f,
     0.97433938f,
     0.97364425f,
     0.97293995f,
     0.97222650f,
     0.97150389f,
     0.97077214f,
     0.97003125f,
     0.96928124f,
     0.96852209f,
     0.96775384f,
     0.96697647f,
     0.96619000f,
     0.96539444f,
     0.96458979f,
     0.96377607f,
     0.96295327f,
     0.96212140f,
     0.96128049f,
     0.96043052f,
     0.95957151f,
     0.95870347f,
     0.95782641f,
     0.95694034f,
     0.95604525f,
     0.95514117f,
     0.95422810f,
     0.95330604f,
     0.95237501f,
     0.95143502f,
     0.95048607f,
     0.94952818f,
     0.94856135f,
     0.94758559f,
     0.94660091f,
     0.94560733f,
     0.94460484f,
     0.94359346f,
     0.94257320f,
     0.94154407f,
     0.94050607f,
     0.93945922f,
     0.93840353f,
     0.93733901f,
     0.93626567f,
     0.93518351f,
     0.93409255f,
     0.93299280f,
     0.93188427f,
     0.93076696f,
     0.92964090f,
     0.92850608f,
     0.92736253f,
     0.92621024f,
     0.92504924f,
     0.92387953f,
     0.92270113f,
     0.92151404f,
     0.92031828f,
     0.91911385f,
     0.91790078f,
     0.91667906f,
     0.91544872f,
     0.91420976f,
     0.91296219f,
     0.91170603f,
     0.91044129f,
     0.90916798f,
     0.90788612f,
     0.90659570f,
     0.90529676f,
     0.90398929f,
     0.90267332f,
     0.90134885f,
     0.90001589f,
     0.89867447f,
     0.89732458f,
     0.89596625f,
     0.89459949f,
     0.89322430f,
     0.89184071f,
     0.89044872f,
     0.88904836f,
     0.88763962f,
     0.88622253f,
     0.88479710f,
     0.88336334f,
     0.88192126f,
     0.88047089f,
     0.87901223f,
     0.87754529f,
     0.87607009f,
     0.87458665f,
     0.87309498f,
     0.87159509f,
     0.87008699f,
     0.86857071f,
     0.86704625f,
     0.86551362f,
     0.86397286f,
     0.86242396f,
     0.86086694f,
     0.85930182f,
     0.85772861f,
     0.85614733f,
     0.85455799f,
     0.85296060f,
     0.85135519f,
     0.84974177f,
     0.84812034f,
     0.84649094f,
     0.84485357f,
     0.84320824f,
     0.84155498f,
     0.83989379f,
     0.83822471f,
     0.83654773f,
     0.83486287f,
     0.83317016f,
     0.83146961f,
     0.82976123f,
     0.82804505f,
     0.82632106f,
     0.82458930f,
     0.82284978f,
     0.82110251f,
     0.81934752f,
     0.81758481f,
     0.81581441f,
     0.81403633f,
     0.81225059f,
     0.81045720f,
     0.80865618f,
     0.80684755f,
     0.80503133f,
     0.80320753f,
     0.80137617f,
     0.79953727f,
     0.79769084f,
     0.79583690f,
     0.79397548f,
     0.79210658f,
     0.79023022f,
     0.78834643f,
     0.78645521f,
     0.78455660f,
     0.78265060f,
     0.78073723f,
     0.77881651f,
     0.77688847f,
     0.77495311f,
     0.77301045f,
     0.77106052f,
     0.76910334f,
     0.76713891f,
     0.76516727f,
     0.76318842f,
     0.76120239f,
     0.75920919f,
     0.75720885f,
     0.75520138f,
     0.75318680f,
     0.75116513f,
     0.74913639f,
     0.74710061f,
     0.74505779f,
     0.74300795f,
     0.74095113f,
     0.73888732f,
     0.73681657f,
     0.73473888f,
     0.73265427f,
     0.73056277f,
     0.72846439f,
     0.72635916f,
     0.72424708f,
     0.72212819f,
     0.72000251f,
     0.71787005f,
     0.71573083f,
     0.71358487f,
     0.71143220f,
     0.70927283f,
     0.70710678f,
     0.70493408f,
     0.70275474f,
     0.70056879f,
     0.69837625f,
     0.69617713f,
     0.69397146f,
     0.69175926f,
     0.68954054f,
     0.68731534f,
     0.68508367f,
     0.68284555f,
     0.68060100f,
     0.67835004f,
     0.67609270f,
     0.67382900f,
     0.67155895f,
     0.66928259f,
     0.66699992f,
     0.66471098f,
     0.66241578f,
     0.66011434f,
     0.65780669f,
     0.65549285f,
     0.65317284f,
     0.65084668f,
     0.64851440f,
     0.64617601f,
     0.64383154f,
     0.64148101f,
     0.63912444f,
     0.63676186f,
     0.63439328f,
     0.63201874f,
     0.62963824f,
     0.62725182f,
     0.62485949f,
     0.62246128f,
     0.62005721f,
     0.61764731f,
     0.61523159f,
     0.61281008f,
     0.61038281f,
     0.60794978f,
     0.60551104f,
     0.60306660f,
     0.60061648f,
     0.59816071f,
     0.59569930f,
     0.59323230f,
     0.59075970f,
     0.58828155f,
     0.58579786f,
     0.58330865f,
     0.58081396f,
     0.57831380f,
     0.57580819f,
     0.57329717f,
     0.57078075f,
     0.56825895f,
     0.56573181f,
     0.56319934f,
     0.56066158f,
     0.55811853f,
     0.55557023f,
     0.55301671f,
     0.55045797f,
     0.54789406f,
     0.54532499f,
     0.54275078f,
     0.54017147f,
     0.53758708f,
     0.53499762f,
     0.53240313f,
     0.52980362f,
     0.52719913f,
     0.52458968f,
     0.52197529f,
     0.51935599f,
     0.51673180f,
     0.51410274f,
     0.51146885f,
     0.50883014f,
     0.50618665f,
     0.50353838f,
     0.50088538f,
     0.49822767f,
     0.49556526f,
     0.49289819f,
     0.49022648f,
     0.48755016f,
     0.48486925f,
     0.48218377f,
     0.47949376f,
     0.47679923f,
     0.47410021f,
     0.47139674f,
     0.46868882f,
     0.46597650f,
     0.46325978f,
     0.46053871f,
     0.45781330f,
     0.45508359f,
     0.45234959f,
     0.44961133f,
     0.44686884f,
     0.44412214f,
     0.44137127f,
     0.43861624f,
     0.43585708f,
     0.43309382f,
     0.43032648f,
     0.42755509f,
     0.42477968f,
     0.42200027f,
     0.41921689f,
     0.41642956f,
     0.41363831f,
     0.41084317f,
     0.40804416f,
     0.40524131f,
     0.40243465f,
     0.39962420f,
     0.39680999f,
     0.39399204f,
     0.39117038f,
     0.38834505f,
     0.38551605f,
     0.38268343f,
     0.37984721f,
     0.37700741f,
     0.37416406f,
     0.37131719f,
     0.36846683f,
     0.36561300f,
     0.36275572f,
     0.35989504f,
     0.35703096f,
     0.35416353f,
     0.35129276f,
     0.34841868f,
     0.34554132f,
     0.34266072f,
     0.33977688f,
     0.33688985f,
     0.33399965f,
     0.33110631f,
     0.32820984f,
     0.32531029f,
     0.32240768f,
     0.31950203f,
     0.31659338f,
     0.31368174f,
     0.31076715f,
     0.30784964f,
     0.30492923f,
     0.30200595f,
     0.29907983f,
     0.29615089f,
     0.29321916f,
     0.29028468f,
     0.28734746f,
     0.28440754f,
     0.28146494f,
     0.27851969f,
     0.27557182f,
     0.27262136f,
     0.26966833f,
     0.26671276f,
     0.26375468f,
     0.26079412f,
     0.25783110f,
     0.25486566f,
     0.25189782f,
     0.24892761f,
     0.24595505f,
     0.24298018f,
     0.24000302f,
     0.23702361f,
     0.23404196f,
     0.23105811f,
     0.22807208f,
     0.22508391f,
     0.22209362f,
     0.21910124f,
     0.21610680f,
     0.21311032f,
     0.21011184f,
     0.20711138f,
     0.20410897f,
     0.20110463f,
     0.19809841f,
     0.19509032f,
     0.19208040f,
     0.18906866f,
     0.18605515f,
     0.18303989f,
     0.18002290f,
     0.17700422f,
     0.17398387f,
     0.17096189f,
     0.16793829f,
     0.16491312f,
     0.16188639f,
     0.15885814f,
     0.15582840f,
     0.15279719f,
     0.14976453f,
     0.14673047f,
     0.14369503f,
     0.14065824f,
     0.13762012f,
     0.13458071f,
     0.13154003f,
     0.12849811f,
     0.12545498f,
     0.12241068f,
     0.11936521f,
     0.11631863f,
     0.11327095f,
     0.11022221f,
     0.10717242f,
     0.10412163f,
     0.10106986f,
     0.09801714f,
     0.09496350f,
     0.09190896f,
     0.08885355f,
     0.08579731f,
     0.08274026f,
     0.07968244f,
     0.07662386f,
     0.07356456f,
     0.07050457f,
     0.06744392f,
     0.06438263f,
     0.06132074f,
     0.05825826f,
     0.05519524f,
     0.05213170f,
     0.04906767f,
     0.04600318f,
     0.04293826f,
     0.03987293f,
     0.03680722f,
     0.03374117f,
     0.03067480f,
     0.02760815f,
     0.02454123f,
     0.02147408f,
     0.01840673f,
     0.01533921f,
     0.01227154f,
     0.00920375f,
     0.00613588f,
     0.00306796f,
     0.00000000f,
    -0.00306796f,
    -0.00613588f,
    -0.00920375f,
    -0.01227154f,
    -0.01533921f,
    -0.01840673f,
    -0.02147408f,
    -0.02454123f,
    -0.02760815f,
    -0.03067480f,
    -0.03374117f,
    -0.03680722f,
    -0.03987293f,
    -0.04293826f,
    -0.04600318f,
    -0.04906767f,
    -0.05213170f,
    -0.05519524f,
    -0.05825826f,
    -0.06132074f,
    -0.06438263f,
    -0.06744392f,
    -0.07050457f,
    -0.07356456f,
    -0.07662386f,
    -0.07968244f,
    -0.08274026f,
    -0.08579731f,
    -0.08885355f,
    -0.09190896f,
    -0.09496350f,
    -0.09801714f,
    -0.10106986f,
    -0.10412163f,
    -0.10717242f,
    -0.11022221f,
    -0.11327095f,
    -0.11631863f,
    -0.11936521f,
    -0.12241068f,
    -0.12545498f,
    -0.12849811f,
    -0.13154003f,
    -0.13458071f,
    -0.13762012f,
    -0.14065824f,
    -0.14369503f,
    -0.14673047f,
    -0.14976453f,
    -0.15279719f,
    -0.15582840f,
    -0.15885814f,
    -0.16188639f,
    -0.16491312f,
    -0.16793829f,
    -0.17096189f,
    -0.17398387f,
    -0.17700422f,
    -0.18002290f,
    -0.18303989f,
    -0.18605515f,
    -0.18906866f,
    -0.19208040f,
    -0.19509032f,
    -0.19809841f,
    -0.20110463f,
    -0.20410897f,
    -0.20711138f,
    -0.21011184f,
    -0.21311032f,
    -0.21610680f,
    -0.21910124f,
    -0.22209362f,
    -0.22508391f,
    -0.22807208f,
    -0.23105811f,
    -0.23404196f,
    -0.23702361f,
    -0.24000302f,
    -0.24298018f,
    -0.24595505f,
    -0.24892761f,
    -0.25189782f,
    -0.25486566f,
    -0.25783110f,
    -0.26079412f,
    -0.26375468f,
    -0.26671276f,
    -0.26966833f,
    -0.27262136f,
    -0.27557182f,
    -0.27851969f,
    -0.28146494f,
    -0.28440754f,
    -0.28734746f,
    -0.29028468f,
    -0.29321916f,
    -0.29615089f,
    -0.29907983f,
    -0.30200595f,
    -0.30492923f,
    -0.30784964f,
    -0.31076715f,
    -0.31368174f,
    -0.31659338f,
    -0.31950203f,
    -0.32240768f,
    -0.32531029f,
    -0.32820984f,
    -0.33110631f,
    -0.33399965f,
    -0.33688985f,
    -0.33977688f,
    -0.34266072f,
    -0.34554132f,
    -0.34841868f,
    -0.35129276f,
    -0.35416353f,
    -0.35703096f,
    -0.35989504f,
    -0.36275572f,
    -0.36561300f,
    -0.36846683f,
    -0.37131719f,
    -0.37416406f,
    -0.37700741f,
    -0.37984721f,
    -0.38268343f,
    -0.38551605f,
    -0.38834505f,
    -0.39117038f,
    -0.39399204f,
    -0.39680999f,
    -0.39962420f,
    -0.40243465f,
    -0.40524131f,
    -0.40804416f,
    -0.41084317f,
    -0.41363831f,
    -0.41642956f,
    -0.41921689f,
    -0.42200027f,
    -0.42477968f,
    -0.42755509f,
    -0.43032648f,
    -0.43309382f,
    -0.43585708f,
    -0.43861624f,
    -0.44137127f,
    -0.44412214f,
    -0.44686884f,
    -0.44961133f,
    -0.45234959f,
    -0.45508359f,
    -0.45781330f,
    -0.46053871f,
    -0.46325978f,
    -0.46597650f,
    -0.46868882f,
    -0.47139674f,
    -0.47410021f,
    -0.47679923f,
    -0.47949376f,
    -0.48218377f,
    -0.48486925f,
    -0.48755016f,
    -0.49022648f,
    -0.49289819f,
    -0.49556526f,
    -0.49822767f,
    -0.50088538f,
    -0.50353838f,
    -0.50618665f,
    -0.50883014f,
    -0.51146885f,
    -0.51410274f,
    -0.51673180f,
    -0.51935599f,
    -0.52197529f,
    -0.52458968f,
    -0.52719913f,
    -0.52980362f,
    -0.53240313f,
    -0.53499762f,
    -0.53758708f,
    -0.54017147f,
    -0.54275078f,
    -0.54532499f,
    -0.54789406f,
    -0.55045797f,
    -0.55301671f,
    -0.55557023f,
    -0.55811853f,
    -0.56066158f,
    -0.56319934f,
    -0.56573181f,
    -0.56825895f,
    -0.57078075f,
    -0.57329717f,
    -0.57580819f,
    -0.57831380f,
    -0.58081396f,
    -0.58330865f,
    -0.58579786f,
    -0.58828155f,
    -0.59075970f,
    -0.59323230f,
    -0.59569930f,
    -0.59816071f,
    -0.60061648f,
    -0.60306660f,
    -0.60551104f,
    -0.60794978f,
    -0.61038281f,
    -0.61281008f,
    -0.61523159f,
    -0.61764731f,
    -0.62005721f,
    -0.62246128f,
    -0.62485949f,
    -0.62725182f,
    -0.62963824f,
    -0.63201874f,
    -0.63439328f,
    -0.63676186f,
    -0.63912444f,
    -0.64148101f,
    -0.64383154f,
    -0.64617601f,
    -0.64851440f,
    -0.65084668f,
    -0.65317284f,
    -0.65549285f,
    -0.65780669f,
    -0.66011434f,
    -0.66241578f,
    -0.66471098f,
    -0.66699992f,
    -0.66928259f,
    -0.67155895f,
    -0.67382900f,
    -0.67609270f,
    -0.67835004f,
    -0.68060100f,
    -0.68284555f,
    -0.68508367f,
    -0.68731534f,
    -0.68954054f,
    -0.69175926f,
    -0.69397146f,
    -0.69617713f,
    -0.69837625f,
    -0.70056879f,
    -0.70275474f,
    -0.70493408f,
    -0.70710678f,
    -0.70927283f,
    -0.71143220f,
    -0.71358487f,
    -0.71573083f,
    -0.71787005f,
    -0.72000251f,
    -0.72212819f,
    -0.72424708f,
    -0.72635916f,
    -0.72846439f,
    -0.73056277f,
    -0.73265427f,
    -0.73473888f,
    -0.73681657f,
    -0.73888732f,
    -0.74095113f,
    -0.74300795f,
    -0.74505779f,
    -0.74710061f,
    -0.74913639f,
    -0.75116513f,
    -0.75318680f,
    -0.75520138f,
    -0.75720885f,
    -0.75920919f,
    -0.76120239f,
    -0.76318842f,
    -0.76516727f,
    -0.76713891f,
    -0.76910334f,
    -0.77106052f,
    -0.77301045f,
    -0.77495311f,
    -0.77688847f,
    -0.77881651f,
    -0.78073723f,
    -0.78265060f,
    -0.78455660f,
    -0.78645521f,
    -0.78834643f,
    -0.79023022f,
    -0.79210658f,
    -0.79397548f,
    -0.79583690f,
    -0.79769084f,
    -0.79953727f,
    -0.80137617f,
    -0.80320753f,
    -0.80503133f,
    -0.80684755f,
    -0.80865618f,
    -0.81045720f,
    -0.81225059f,
    -0.81403633f,
    -0.81581441f,
    -0.81758481f,
    -0.81934752f,
    -0.82110251f,
    -0.82284978f,
    -0.82458930f,
    -0.82632106f,
    -0.82804505f,
    -0.82976123f,
    -0.83146961f,
    -0.83317016f,
    -0.83486287f,
    -0.83654773f,
    -0.83822471f,
    -0.83989379f,
    -0.84155498f,
    -0.84320824f,
    -0.84485357f,
    -0.84649094f,
    -0.84812034f,
    -0.84974177f,
    -0.85135519f,
    -0.85296060f,
    -0.85455799f,
    -0.85614733f,
    -0.85772861f,
    -0.85930182f,
    -0.86086694f,
    -0.86242396f,
    -0.86397286f,
    -0.86551362f,
    -0.86704625f,
    -0.86857071f,
    -0.87008699f,
    -0.87159509f,
    -0.87309498f,
    -0.87458665f,
    -0.87607009f,
    -0.87754529f,
    -0.87901223f,
    -0.88047089f,
    -0.88192126f,
    -0.88336334f,
    -0.88479710f,
    -0.88622253f,
    -0.88763962f,
    -0.88904836f,
    -0.89044872f,
    -0.89184071f,
    -0.89322430f,
    -0.89459949f,
    -0.89596625f,
    -0.89732458f,
    -0.89867447f,
    -0.90001589f,
    -0.90134885f,
    -0.90267332f,
    -0.90398929f,
    -0.90529676f,
    -0.90659570f,
    -0.90788612f,
    -0.90916798f,
    -0.91044129f,
    -0.91170603f,
    -0.91296219f,
    -0.91420976f,
    -0.91544872f,
    -0.91667906f,
    -0.91790078f,
    -0.91911385f,
    -0.92031828f,
    -0.92151404f,
    -0.92270113f,
    -0.92387953f,
    -0.92504924f,
    -0.92621024f,
    -0.92736253f,
    -0.92850608f,
    -0.92964090f,
    -0.93076696f,
    -0.93188427f,
    -0.93299280f,
    -0.93409255f,
    -0.93518351f,
    -0.93626567f,
    -0.93733901f,
    -0.93840353f,
    -0.93945922f,
    -0.94050607f,
    -0.94154407f,
    -0.94257320f,
    -0.94359346f,
    -0.94460484f,
    -0.94560733f,
    -0.94660091f,
    -0.94758559f,
    -0.94856135f,
    -0.94952818f,
    -0.95048607f,
    -0.95143502f,
    -0.95237501f,
    -0.95330604f,
    -0.95422810f,
    -0.95514117f,
    -0.95604525f,
    -0.95694034f,
    -0.95782641f,
    -0.95870347f,
    -0.95957151f,
    -0.96043052f,
    -0.96128049f,
    -0.96212140f,
    -0.96295327f,
    -0.96377607f,
    -0.96458979f,
    -0.96539444f,
    -0.96619000f,
    -0.96697647f,
    -0.96775384f,
    -0.96852209f,
    -0.96928124f,
    -0.97003125f,
    -0.97077214f,
    -0.97150389f,
    -0.97222650f,
    -0.97293995f,
    -0.97364425f,
    -0.97433938f,
    -0.97502535f,
    -0.97570213f,
    -0.97636973f,
    -0.97702814f,
    -0.97767736f,
    -0.97831737f,
    -0.97894818f,
    -0.97956977f,
    -0.98018214f,
    -0.98078528f,
    -0.98137919f,
    -0.98196387f,
    -0.98253930f,
    -0.98310549f,
    -0.98366242f,
    -0.98421009f,
    -0.98474850f,
    -0.98527764f,
    -0.98579751f,
    -0.98630810f,
    -0.98680940f,
    -0.98730142f,
    -0.98778414f,
    -0.98825757f,
    -0.98872169f,
    -0.98917651f,
    -0.98962202f,
    -0.99005821f,
    -0.99048508f,
    -0.99090264f,
    -0.99131086f,
    -0.99170975f,
    -0.99209931f,
    -0.99247953f,
    -0.99285041f,
    -0.99321195f,
    -0.99356414f,
    -0.99390697f,
    -0.99424045f,
    -0.99456457f,
    -0.99487933f,
    -0.99518473f,
    -0.99548076f,
    -0.99576741f,
    -0.99604470f,
    -0.99631261f,
    -0.99657115f,
    -0.99682030f,
    -0.99706007f,
    -0.99729046f,
    -0.99751146f,
    -0.99772307f,
    -0.99792529f,
    -0.99811811f,
    -0.99830154f,
    -0.99847558f,
    -0.99864022f,
    -0.99879546f,
    -0.99894129f,
    -0.99907773f,
    -0.99920476f,
    -0.99932238f,
    -0.99943060f,
    -0.99952942f,
    -0.99961882f,
    -0.99969882f,
    -0.99976941f,
    -0.99983058f,
    -0.99988235f,
    -0.99992470f,
    -0.99995764f,
    -0.99998118f,
    -0.99999529f,
    -1.00000000f,
    -0.99999529f,
    -0.99998118f,
    -0.99995764f,
    -0.99992470f,
    -0.99988235f,
    -0.99983058f,
    -0.99976941f,
    -0.99969882f,
    -0.99961882f,
    -0.99952942f,
    -0.99943060f,
    -0.99932238f,
    -0.99920476f,
    -0.99907773f,
    -0.99894129f,
    -0.99879546f,
    -0.99864022f,
    -0.99847558f,
    -0.99830154f,
    -0.99811811f,
    -0.99792529f,
    -0.99772307f,
    -0.99751146f,
    -0.99729046f,
    -0.99706007f,
    -0.99682030f,
    -0.99657115f,
    -0.99631261f,
    -0.99604470f,
    -0.99576741f,
    -0.99548076f,
    -0.99518473f,
    -0.99487933f,
    -0.99456457f,
    -0.99424045f,
    -0.99390697f,
    -0.99356414f,
    -0.99321195f,
    -0.99285041f,
    -0.99247953f,
    -0.99209931f,
    -0.99170975f,
    -0.99131086f,
    -0.99090264f,
    -0.99048508f,
    -0.99005821f,
    -0.98962202f,
    -0.98917651f,
    -0.98872169f,
    -0.98825757f,
    -0.98778414f,
    -0.98730142f,
    -0.98680940f,
    -0.98630810f,
    -0.98579751f,
    -0.98527764f,
    -0.98474850f,
    -0.98421009f,
    -0.98366242f,
    -0.98310549f,
    -0.98253930f,
    -0.98196387f,
    -0.98137919f,
    -0.98078528f,
    -0.98018214f,
    -0.97956977f,
    -0.97894818f,
    -0.97831737f,
    -0.97767736f,
    -0.97702814f,
    -0.97636973f,
    -0.97570213f,
    -0.97502535f,
    -0.97433938f,
    -0.97364425f,
    -0.97293995f,
    -0.97222650f,
    -0.97150389f,
    -0.97077214f,
    -0.97003125f,
    -0.96928124f,
    -0.96852209f,
    -0.96775384f,
    -0.96697647f,
    -0.96619000f,
    -0.96539444f,
    -0.96458979f,
    -0.96377607f,
    -0.96295327f,
    -0.96212140f,
    -0.96128049f,
    -0.96043052f,
    -0.95957151f,
    -0.95870347f,
    -0.95782641f,
    -0.95694034f,
    -0.95604525f,
    -0.95514117f,
    -0.95422810f,
    -0.95330604f,
    -0.95237501f,
    -0.95143502f,
    -0.95048607f,
    -0.94952818f,
    -0.94856135f,
    -0.94758559f,
    -0.94660091f,
    -0.94560733f,
    -0.94460484f,
    -0.94359346f,
    -0.94257320f,
    -0.94154407f,
    -0.94050607f,
    -0.93945922f,
    -0.93840353f,
    -0.93733901f,
    -0.93626567f,
    -0.93518351f,
    -0.93409255f,
    -0.93299280f,
    -0.93188427f,
    -0.93076696f,
    -0.92964090f,
    -0.92850608f,
    -0.92736253f,
    -0.92621024f,
    -0.92504924f,
    -0.92387953f,
    -0.92270113f,
    -0.92151404f,
    -0.92031828f,
    -0.91911385f,
    -0.91790078f,
    -0.91667906f,
    -0.91544872f,
    -0.91420976f,
    -0.91296219f,
    -0.91170603f,
    -0.91044129f,
    -0.90916798f,
    -0.90788612f,
    -0.90659570f,
    -0.90529676f,
    -0.90398929f,
    -0.90267332f,
    -0.90134885f,
    -0.90001589f,
    -0.89867447f,
    -0.89732458f,
    -0.89596625f,
    -0.89459949f,
    -0.89322430f,
    -0.89184071f,
    -0.89044872f,
    -0.88904836f,
    -0.88763962f,
    -0.88622253f,
    -0.88479710f,
    -0.88336334f,
    -0.88192126f,
    -0.88047089f,
    -0.87901223f,
    -0.87754529f,
    -0.87607009f,
    -0.87458665f,
    -0.87309498f,
    -0.87159509f,
    -0.87008699f,
    -0.86857071f,
    -0.86704625f,
    -0.86551362f,
    -0.86397286f,
    -0.86242396f,
    -0.86086694f,
    -0.85930182f,
    -0.85772861f,
    -0.85614733f,
    -0.85455799f,
    -0.85296060f,
    -0.85135519f,
    -0.84974177f,
    -0.84812034f,
    -0.84649094f,
    -0.84485357f,
    -0.84320824f,
    -0.84155498f,
    -0.83989379f,
    -0.83822471f,
    -0.83654773f,
    -0.83486287f,
    -0.83317016f,
    -0.83146961f,
    -0.82976123f,
    -0.82804505f,
    -0.82632106f,
    -0.82458930f,
    -0.82284978f,
    -0.82110251f,
    -0.81934752f,
    -0.81758481f,
    -0.81581441f,
    -0.81403633f,
    -0.81225059f,
    -0.81045720f,
    -0.80865618f,
    -0.80684755f,
    -0.80503133f,
    -0.80320753f,
    -0.80137617f,
    -0.79953727f,
    -0.79769084f,
    -0.79583690f,
    -0.79397548f,
    -0.79210658f,
    -0.79023022f,
    -0.78834643f,
    -0.78645521f,
    -0.78455660f,
    -0.78265060f,
    -0.78073723f,
    -0.77881651f,
    -0.77688847f,
    -0.77495311f,
    -0.77301045f,
    -0.77106052f,
    -0.76910334f,
    -0.76713891f,
    -0.76516727f,
    -0.76318842f,
    -0.76120239f,
    -0.75920919f,
    -0.75720885f,
    -0.75520138f,
    -0.75318680f,
    -0.75116513f,
    -0.74913639f,
    -0.74710061f,
    -0.74505779f,
    -0.74300795f,
    -0.74095113f,
    -0.73888732f,
    -0.73681657f,
    -0.73473888f,
    -0.73265427f,
    -0.73056277f,
    -0.72846439f,
    -0.72635916f,
    -0.72424708f,
    -0.72212819f,
    -0.72000251f,
    -0.71787005f,
    -0.71573083f,
    -0.71358487f,
    -0.71143220f,
    -0.70927283f,
    -0.70710678f,
    -0.70493408f,
    -0.70275474f,
    -0.70056879f,
    -0.69837625f,
    -0.69617713f,
    -0.69397146f,
    -0.69175926f,
    -0.68954054f,
    -0.68731534f,
    -0.68508367f,
    -0.68284555f,
    -0.68060100f,
    -0.67835004f,
    -0.67609270f,
    -0.67382900f,
    -0.67155895f,
    -0.66928259f,
    -0.66699992f,
    -0.66471098f,
    -0.66241578f,
    -0.66011434f,
    -0.65780669f,
    -0.65549285f,
    -0.65317284f,
    -0.65084668f,
    -0.64851440f,
    -0.64617601f,
    -0.64383154f,
    -0.64148101f,
    -0.63912444f,
    -0.63676186f,
    -0.63439328f,
    -0.63201874f,
    -0.62963824f,
    -0.62725182f,
    -0.62485949f,
    -0.62246128f,
    -0.62005721f,
    -0.61764731f,
    -0.61523159f,
    -0.61281008f,
    -0.61038281f,
    -0.60794978f,
    -0.60551104f,
    -0.60306660f,
    -0.60061648f,
    -0.59816071f,
    -0.59569930f,
    -0.59323230f,
    -0.59075970f,
    -0.58828155f,
    -0.58579786f,
    -0.58330865f,
    -0.58081396f,
    -0.57831380f,
    -0.57580819f,
    -0.57329717f,
    -0.57078075f,
    -0.56825895f,
    -0.56573181f,
    -0.56319934f,
    -0.56066158f,
    -0.55811853f,
    -0.55557023f,
    -0.55301671f,
    -0.55045797f,
    -0.54789406f,
    -0.54532499f,
    -0.54275078f,
    -0.54017147f,
    -0.53758708f,
    -0.53499762f,
    -0.53240313f,
    -0.52980362f,
    -0.52719913f,
    -0.52458968f,
    -0.52197529f,
    -0.51935599f,
    -0.51673180f,
    -0.51410274f,
    -0.51146885f,
    -0.50883014f,
    -0.50618665f,
    -0.50353838f,
    -0.50088538f,
    -0.49822767f,
    -0.49556526f,
    -0.49289819f,
    -0.49022648f,
    -0.48755016f,
    -0.48486925f,
    -0.48218377f,
    -0.47949376f,
    -0.47679923f,
    -0.47410021f,
    -0.47139674f,
    -0.46868882f,
    -0.46597650f,
    -0.46325978f,
    -0.46053871f,
    -0.45781330f,
    -0.45508359f,
    -0.45234959f,
    -0.44961133f,
    -0.44686884f,
    -0.44412214f,
    -0.44137127f,
    -0.43861624f,
    -0.43585708f,
    -0.43309382f,
    -0.43032648f,
    -0.42755509f,
    -0.42477968f,
    -0.42200027f,
    -0.41921689f,
    -0.41642956f,
    -0.41363831f,
    -0.41084317f,
    -0.40804416f,
    -0.40524131f,
    -0.40243465f,
    -0.39962420f,
    -0.39680999f,
    -0.39399204f,
    -0.39117038f,
    -0.38834505f,
    -0.38551605f,
    -0.38268343f,
    -0.37984721f,
    -0.37700741f,
    -0.37416406f,
    -0.37131719f,
    -0.36846683f,
    -0.36561300f,
    -0.36275572f,
    -0.35989504f,
    -0.35703096f,
    -0.35416353f,
    -0.35129276f,
    -0.34841868f,
    -0.34554132f,
    -0.34266072f,
    -0.33977688f,
    -0.33688985f,
    -0.33399965f,
    -0.33110631f,
    -0.32820984f,
    -0.32531029f,
    -0.32240768f,
    -0.31950203f,
    -0.31659338f,
    -0.31368174f,
    -0.31076715f,
    -0.30784964f,
    -0.30492923f,
    -0.30200595f,
    -0.29907983f,
    -0.29615089f,
    -0.29321916f,
    -0.29028468f,
    -0.28734746f,
    -0.28440754f,
    -0.28146494f,
    -0.27851969f,
    -0.27557182f,
    -0.27262136f,
    -0.26966833f,
    -0.26671276f,
    -0.26375468f,
    -0.26079412f,
    -0.25783110f,
    -0.25486566f,
    -0.25189782f,
    -0.24892761f,
    -0.24595505f,
    -0.24298018f,
    -0.24000302f,
    -0.23702361f,
    -0.23404196f,
    -0.23105811f,
    -0.22807208f,
    -0.22508391f,
    -0.22209362f,
    -0.21910124f,
    -0.21610680f,
    -0.21311032f,
    -0.21011184f,
    -0.20711138f,
    -0.20410897f,
    -0.20110463f,
    -0.19809841f,
    -0.19509032f,
    -0.19208040f,
    -0.18906866f,
    -0.18605515f,
    -0.18303989f,
    -0.18002290f,
    -0.17700422f,
    -0.17398387f,
    -0.17096189f,
    -0.16793829f,
    -0.16491312f,
    -0.16188639f,
    -0.15885814f,
    -0.15582840f,
    -0.15279719f,
    -0.14976453f,
    -0.14673047f,
    -0.14369503f,
    -0.14065824f,
    -0.13762012f,
    -0.13458071f,
    -0.13154003f,
    -0.12849811f,
    -0.12545498f,
    -0.12241068f,
    -0.11936521f,
    -0.11631863f,
    -0.11327095f,
    -0.11022221f,
    -0.10717242f,
    -0.10412163f,
    -0.10106986f,
    -0.09801714f,
    -0.09496350f,
    -0.09190896f,
    -0.08885355f,
    -0.08579731f,
    -0.08274026f,
    -0.07968244f,
    -0.07662386f,
    -0.07356456f,
    -0.07050457f,
    -0.06744392f,
    -0.06438263f,
    -0.06132074f,
    -0.05825826f,
    -0.05519524f,
    -0.05213170f,
    -0.04906767f,
    -0.04600318f,
    -0.04293826f,
    -0.03987293f,
    -0.03680722f,
    -0.03374117f,
    -0.03067480f,
    -0.02760815f,
    -0.02454123f,
    -0.02147408f,
    -0.01840673f,
    -0.01533921f,
    -0.01227154f,
    -0.00920375f,
    -0.00613588f,
    -0.00306796f
};

static int32_t dds_phase_ratef(float frequency)
{
	return (int32_t) (frequency*65536.0f*65536.0f/SAMPLE_RATE);
}

static float dds_scaling_dbm0f(float level)
{
	return powf(10.0f, (level - DBM0_MAX_SINE_POWER)/20.0f)*32767.0f;
}

static float dds_modf(uint32_t *phase_acc, int32_t phase_rate, float scale, int32_t phase)
{
	float amp;
	amp = sine_table[*(phase_acc + phase) >> (32 - SLENK)]*scale;
	*phase_acc += phase_rate;
	return amp;
}


/******* DTMF routines ********/

/* Tx specs */
#define DEFAULT_DTMF_TX_LEVEL       -10
#define DEFAULT_DTMF_TX_ON_TIME     50
#define DEFAULT_DTMF_TX_OFF_TIME    55

/* Rx specs */
#define DTMF_THRESHOLD              8.0e7f
#define DTMF_NORMAL_TWIST           6.3f    /* 8dB */
#define DTMF_REVERSE_TWIST          2.5f    /* 4dB */
#define DTMF_RELATIVE_PEAK_ROW      6.3f    /* 8dB */
#define DTMF_RELATIVE_PEAK_COL      6.3f    /* 8dB */
#define DTMF_TO_TOTAL_ENERGY        42.0f
#define DTMF_POWER_OFFSET           90.30f

/* This is based on A-law, but u-law is only 0.03dB different */
#define DBM0_MAX_POWER          (3.14f + 3.02f)

static const float dtmf_row[] =
{
     697.0f,  770.0f,  852.0f,  941.0f
};
static const float dtmf_col[] =
{
    1209.0f, 1336.0f, 1477.0f, 1633.0f
};

static const char dtmf_positions[] = "123A" "456B" "789C" "*0#D";
static openr2_goertzel_descriptor_t dtmf_detect_row[4];
static openr2_goertzel_descriptor_t dtmf_detect_col[4];

static int dtmf_tx_inited = FALSE;
static openr2_tone_gen_descriptor_t dtmf_digit_tones[16];

static void dtmf_tx_initialise(void)
{
    int row;
    int col;

    if (dtmf_tx_inited)
        return;
    for (row = 0;  row < 4;  row++)
    {
        for (col = 0;  col < 4;  col++)
        {
            make_tone_gen_descriptor(&dtmf_digit_tones[row*4 + col],
                                     (int) dtmf_row[row],
                                     DEFAULT_DTMF_TX_LEVEL,
                                     (int) dtmf_col[col],
                                     DEFAULT_DTMF_TX_LEVEL,
                                     DEFAULT_DTMF_TX_ON_TIME,
                                     DEFAULT_DTMF_TX_OFF_TIME,
                                     0,
                                     0,
                                     FALSE);
        }
    }
    dtmf_tx_inited = TRUE;
}

OR2_EXPORT_SYMBOL
void openr2_dtmf_tx_set_level(openr2_dtmf_tx_state_t *s, int level, int twist)
{
    s->low_level = dds_scaling_dbm0f((float) level);
    s->high_level = dds_scaling_dbm0f((float) (level + twist));
}

OR2_EXPORT_SYMBOL
openr2_dtmf_tx_state_t *openr2_dtmf_tx_init(openr2_dtmf_tx_state_t *s)
{
    if (s == NULL)
    {
        if ((s = (openr2_dtmf_tx_state_t *) malloc(sizeof (*s))) == NULL)
            return  NULL;
    }
    if (!dtmf_tx_inited)
        dtmf_tx_initialise();
    tone_gen_init(&(s->tones), &dtmf_digit_tones[0]);
    openr2_dtmf_tx_set_level(s, DEFAULT_DTMF_TX_LEVEL, 0);
    openr2_dtmf_tx_set_timing(s, -1, -1);
    queue_init(&s->queue.queue, OR2_MAX_DTMF_DIGITS, QUEUE_READ_ATOMIC | QUEUE_WRITE_ATOMIC);
    s->tones.current_section = -1;
    return s;
}

OR2_EXPORT_SYMBOL
void openr2_dtmf_tx_set_timing(openr2_dtmf_tx_state_t *s, int on_time, int off_time)
{
    s->on_time = ((on_time >= 0)  ?  on_time  :  DEFAULT_DTMF_TX_ON_TIME)*SAMPLE_RATE/1000;
    s->off_time = ((off_time >= 0)  ?  off_time  :  DEFAULT_DTMF_TX_OFF_TIME)*SAMPLE_RATE/1000;
}

OR2_EXPORT_SYMBOL
size_t openr2_dtmf_tx_put(openr2_dtmf_tx_state_t *s, const char *digits, int len)
{
    size_t space;

    /* This returns the number of characters that would not fit in the buffer.
       The buffer will only be loaded if the whole string of digits will fit,
       in which case zero is returned. */
    if (len < 0)
    {
        if ((len = strlen(digits)) == 0)
            return 0;
    }
    if ((space = queue_free_space(&s->queue.queue)) < len)
        return len - space;
    if (queue_write(&s->queue.queue, (const uint8_t *) digits, len) >= 0)
        return 0;
    return -1;
}

OR2_EXPORT_SYMBOL
int openr2_dtmf_tx(openr2_dtmf_tx_state_t *s, int16_t amp[], int max_samples)
{
    int len;
    const char *cp;
    int digit;

    len = 0;
    if (s->tones.current_section >= 0)
    {
        /* Deal with the fragment left over from last time */
        len = tone_gen(&(s->tones), amp, max_samples);
    }
    while (len < max_samples  &&  (digit = queue_read_byte(&s->queue.queue)) >= 0)
    {
        /* Step to the next digit */
        if (digit == 0)
            continue;
        if ((cp = strchr(dtmf_positions, digit)) == NULL)
            continue;
        tone_gen_init(&(s->tones), &dtmf_digit_tones[cp - dtmf_positions]);
        s->tones.tone[0].gain = s->low_level;
        s->tones.tone[1].gain = s->high_level;
        s->tones.duration[0] = s->on_time;
        s->tones.duration[1] = s->off_time;
        len += tone_gen(&(s->tones), amp + len, max_samples - len);
    }
    return len;
}

OR2_EXPORT_SYMBOL
openr2_dtmf_rx_state_t *openr2_dtmf_rx_init(openr2_dtmf_rx_state_t *s,
                              openr2_digits_rx_callback_t callback,
                              void *user_data)
{
    int i;
    static int initialised = FALSE;

    if (s == NULL)
    {
        if ((s = (openr2_dtmf_rx_state_t *) malloc(sizeof (*s))) == NULL)
            return  NULL;
    }
    s->digits_callback = callback;
    s->digits_callback_data = user_data;
    s->realtime_callback = NULL;
    s->realtime_callback_data = NULL;
    s->filter_dialtone = FALSE;
    s->normal_twist = DTMF_NORMAL_TWIST;
    s->reverse_twist = DTMF_REVERSE_TWIST;

    s->in_digit = 0;
    s->last_hit = 0;

    if (!initialised)
    {
        for (i = 0;  i < 4;  i++)
        {
            make_goertzel_descriptor(&dtmf_detect_row[i], dtmf_row[i], 102);
            make_goertzel_descriptor(&dtmf_detect_col[i], dtmf_col[i], 102);
        }
        initialised = TRUE;
    }
    for (i = 0;  i < 4;  i++)
    {
        goertzel_init(&s->row_out[i], &dtmf_detect_row[i]);
        goertzel_init(&s->col_out[i], &dtmf_detect_col[i]);
    }
    s->energy = 0.0f;
    s->current_sample = 0;
    s->lost_digits = 0;
    s->current_digits = 0;
    s->digits[0] = '\0';
    return s;
}

OR2_EXPORT_SYMBOL
int openr2_dtmf_rx(openr2_dtmf_rx_state_t *s, const int16_t amp[], int samples)
{
    float row_energy[4];
    float col_energy[4];
    float famp;
    float v1;
    int i;
    int j;
    int sample;
    int best_row;
    int best_col;
    int limit;
    uint8_t hit;

    hit = 0;
    for (sample = 0;  sample < samples;  sample = limit)
    {
        /* The block length is optimised to meet the DTMF specs. */
        if ((samples - sample) >= (102 - s->current_sample))
            limit = sample + (102 - s->current_sample);
        else
            limit = samples;
        /* The following unrolled loop takes only 35% (rough estimate) of the 
           time of a rolled loop on the machine on which it was developed */
        for (j = sample;  j < limit;  j++)
        {
            famp = amp[j];
            if (s->filter_dialtone)
            {
                /* Sharp notches applied at 350Hz and 440Hz - the two common dialtone frequencies.
                   These are rather high Q, to achieve the required narrowness, without using lots of
                   sections. */
                v1 = 0.98356f*famp + 1.8954426f*s->z350[0] - 0.9691396f*s->z350[1];
                famp = v1 - 1.9251480f*s->z350[0] + s->z350[1];
                s->z350[1] = s->z350[0];
                s->z350[0] = v1;

                v1 = 0.98456f*famp + 1.8529543f*s->z440[0] - 0.9691396f*s->z440[1];
                famp = v1 - 1.8819938f*s->z440[0] + s->z440[1];
                s->z440[1] = s->z440[0];
                s->z440[0] = v1;
            }
            s->energy += famp*famp;
            /* With GCC 2.95, the following unrolled code seems to take about 35%
               (rough estimate) as long as a neat little 0-3 loop */
            v1 = s->row_out[0].v2;
            s->row_out[0].v2 = s->row_out[0].v3;
            s->row_out[0].v3 = s->row_out[0].fac*s->row_out[0].v2 - v1 + famp;

            v1 = s->col_out[0].v2;
            s->col_out[0].v2 = s->col_out[0].v3;
            s->col_out[0].v3 = s->col_out[0].fac*s->col_out[0].v2 - v1 + famp;

            v1 = s->row_out[1].v2;
            s->row_out[1].v2 = s->row_out[1].v3;
            s->row_out[1].v3 = s->row_out[1].fac*s->row_out[1].v2 - v1 + famp;

            v1 = s->col_out[1].v2;
            s->col_out[1].v2 = s->col_out[1].v3;
            s->col_out[1].v3 = s->col_out[1].fac*s->col_out[1].v2 - v1 + famp;

            v1 = s->row_out[2].v2;
            s->row_out[2].v2 = s->row_out[2].v3;
            s->row_out[2].v3 = s->row_out[2].fac*s->row_out[2].v2 - v1 + famp;

            v1 = s->col_out[2].v2;
            s->col_out[2].v2 = s->col_out[2].v3;
            s->col_out[2].v3 = s->col_out[2].fac*s->col_out[2].v2 - v1 + famp;

            v1 = s->row_out[3].v2;
            s->row_out[3].v2 = s->row_out[3].v3;
            s->row_out[3].v3 = s->row_out[3].fac*s->row_out[3].v2 - v1 + famp;

            v1 = s->col_out[3].v2;
            s->col_out[3].v2 = s->col_out[3].v3;
            s->col_out[3].v3 = s->col_out[3].fac*s->col_out[3].v2 - v1 + famp;
        }
        s->current_sample += (limit - sample);
        if (s->current_sample < 102)
            continue;

        /* We are at the end of a DTMF detection block */
        /* Find the peak row and the peak column */
        row_energy[0] = goertzel_result(&s->row_out[0]);
        best_row = 0;
        col_energy[0] = goertzel_result(&s->col_out[0]);
        best_col = 0;

        for (i = 1;  i < 4;  i++)
        {
            row_energy[i] = goertzel_result(&s->row_out[i]);
            if (row_energy[i] > row_energy[best_row])
                best_row = i;
            col_energy[i] = goertzel_result(&s->col_out[i]);
            if (col_energy[i] > col_energy[best_col])
                best_col = i;
        }
        hit = 0;
        /* Basic signal level test and the twist test */
        if (row_energy[best_row] >= DTMF_THRESHOLD
            &&
            col_energy[best_col] >= DTMF_THRESHOLD
            &&
            col_energy[best_col] < row_energy[best_row]*s->reverse_twist
            &&
            col_energy[best_col]*s->normal_twist > row_energy[best_row])
        {
            /* Relative peak test ... */
            for (i = 0;  i < 4;  i++)
            {
                if ((i != best_col  &&  col_energy[i]*DTMF_RELATIVE_PEAK_COL > col_energy[best_col])
                    ||
                    (i != best_row  &&  row_energy[i]*DTMF_RELATIVE_PEAK_ROW > row_energy[best_row]))
                {
                    break;
                }
            }
            /* ... and fraction of total energy test */
            if (i >= 4
                &&
                (row_energy[best_row] + col_energy[best_col]) > DTMF_TO_TOTAL_ENERGY*s->energy)
            {
                /* Got a hit */
                hit = dtmf_positions[(best_row << 2) + best_col];
            }
        }
        /* The logic in the next test should ensure the following for different successive hit patterns:
                -----ABB = start of digit B.
                ----B-BB = start of digit B
                ----A-BB = start of digit B
                BBBBBABB = still in digit B.
                BBBBBB-- = end of digit B
                BBBBBBC- = end of digit B
                BBBBACBB = B ends, then B starts again.
                BBBBBBCC = B ends, then C starts.
                BBBBBCDD = B ends, then D starts.
           This can work with:
                - Back to back differing digits. Back-to-back digits should
                  not happen. The spec. says there should be a gap between digits.
                  However, many real phones do not impose a gap, and rolling across
                  the keypad can produce little or no gap.
                - It tolerates nasty phones that give a very wobbly start to a digit.
                - VoIP can give sample slips. The phase jumps that produces will cause
                  the block it is in to give no detection. This logic will ride over a
                  single missed block, and not falsely declare a second digit. If the
                  hiccup happens in the wrong place on a minimum length digit, however
                  we would still fail to detect that digit. Could anything be done to
                  deal with that? Packet loss is clearly a no-go zone.
                  Note this is only relevant to VoIP using A-law, u-law or similar.
                  Low bit rate codecs scramble DTMF too much for it to be recognised,
                  and often slip in units larger than a sample. */
        if (hit != s->in_digit)
        {
            if (s->last_hit != s->in_digit)
            {
                /* We have two successive indications that something has changed. */
                /* To declare digit on, the hits must agree. Otherwise we declare tone off. */
                hit = (hit  &&  hit == s->last_hit)  ?  hit   :  0;
                if (s->realtime_callback)
                {
                    /* Avoid reporting multiple no digit conditions on flaky hits */
                    if (s->in_digit  ||  hit)
                    {
                        i = (s->in_digit  &&  !hit)  ?  -99  :  rintf(log10f(s->energy)*10.0f - 20.08f - DTMF_POWER_OFFSET + DBM0_MAX_POWER);
                        s->realtime_callback(s->realtime_callback_data, hit, i, 0);
                    }
                }
                else
                {
                    if (hit)
                    {
                        if (s->current_digits < OR2_MAX_DTMF_DIGITS)
                        {
                            s->digits[s->current_digits++] = (char) hit;
                            s->digits[s->current_digits] = '\0';
                            if (s->digits_callback)
                            {
                                s->digits_callback(s->digits_callback_data, s->digits, s->current_digits);
                                s->current_digits = 0;
                            }
                        }
                        else
                        {
                            s->lost_digits++;
                        }
                    }
                }
                s->in_digit = hit;
            }
        }
        s->last_hit = hit;
        /* Reinitialise the detector for the next block */
        for (i = 0;  i < 4;  i++)
        {
            goertzel_reset(&s->row_out[i]);
            goertzel_reset(&s->col_out[i]);
        }
        s->energy = 0.0f;
        s->current_sample = 0;
    }
    if (s->current_digits  &&  s->digits_callback)
    {
        s->digits_callback(s->digits_callback_data, s->digits, s->current_digits);
        s->digits[0] = '\0';
        s->current_digits = 0;
    }
    return 0;
}

OR2_EXPORT_SYMBOL
int openr2_dtmf_rx_status(openr2_dtmf_rx_state_t *s)
{
    if (s->in_digit)
        return s->in_digit;
    if (s->last_hit)
        return 'x';
    return 0;
}

