/* 
  A Minimal Capture/Playback to UAC2 by Kwang Myung Jeon

  Origined from Paul David's tutorial : http://equalarea.com/paul/alsa-audio.html


  Usage (Works on Raspbian)
  sudo apt-get install libasound2-dev
  gcc -o UAC_pcm -lasound UAC_pcm.c
  ./UAC_pcm hw:"seeed2micvoicec" hw:"UAC2Gadget"
*/

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <unistd.h>

#define REC_LOCAL 0
#define UAC_DEBUG 0

//Define Audio Parameters
#define BUFF_LEN 4096
#define NUM_CH 2

main(int argc, char *argv[])
{
  int err;

  //[Variables] preparing sound capture device (physical)========
  char *buffer;
  snd_pcm_t *capture_handle;
  snd_pcm_hw_params_t *c_hw_params;
  snd_pcm_t *playback_handle;
  snd_pcm_hw_params_t *p_hw_params;
  snd_pcm_format_t PCM_FORMAT = SND_PCM_FORMAT_S16_LE;
  unsigned int SAMPLE_RATE_c = 48000;
  unsigned int SAMPLE_RATE_p = 48000;

  //[Variables] for signal processing
  int i;
  short *frm;

#if REC_LOCAL
  FILE *sig_FOut;
#endif

  if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0)
  {
    fprintf(stderr, "cannot open audio capture device %s (%s)\n",
            argv[1],
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "audio interface opened\n");

#if REC_LOCAL
  if ((sig_FOut = fopen(argv[2], "wb")) == NULL)
  {
    printf("cannot open audio file for recording\n");
    exit(1);
  }
  fprintf(stdout, "audio file for recording opened\n");
#endif

  if ((err = snd_pcm_hw_params_malloc(&c_hw_params)) < 0)
  {
    fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params allocated\n");

  if ((err = snd_pcm_hw_params_any(capture_handle, c_hw_params)) < 0)
  {
    fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params initialized\n");

  if ((err = snd_pcm_hw_params_set_access(capture_handle, c_hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    fprintf(stderr, "cannot set access type (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params access setted\n");

  if ((err = snd_pcm_hw_params_set_format(capture_handle, c_hw_params, PCM_FORMAT)) < 0)
  {
    fprintf(stderr, "cannot set sample format (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params format setted\n");

  if ((err = snd_pcm_hw_params_set_rate_near(capture_handle, c_hw_params, &SAMPLE_RATE_c, 0)) < 0)
  {
    fprintf(stderr, "cannot set sample SAMPLE_RATE (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params SAMPLE_RATE setted\n");

  if ((err = snd_pcm_hw_params_set_channels(capture_handle, c_hw_params, NUM_CH)) < 0)
  {
    fprintf(stderr, "cannot set channel count (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params channels setted\n");

  if ((err = snd_pcm_hw_params(capture_handle, c_hw_params)) < 0)
  {
    fprintf(stderr, "cannot set parameters (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "c_hw_params setted\n");

  snd_pcm_hw_params_free(c_hw_params);
  fprintf(stdout, "c_hw_params freed\n");

  if ((err = snd_pcm_prepare(capture_handle)) < 0)
  {
    fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
            snd_strerror(err));
    exit(1);
  }
  fprintf(stdout, "audio capture interface prepared\n");
  //=====================================================================

#if REC_LOCAL == 0
  //Preparing sound capture device (UAC2)========

  if ((err = snd_pcm_open(&playback_handle, argv[2], SND_PCM_STREAM_PLAYBACK, 0)) < 0)
  {
    fprintf(stderr, "cannot open audio play device %s (%s)\n",
            argv[2],
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params_malloc(&p_hw_params)) < 0)
  {
    fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params_any(playback_handle, p_hw_params)) < 0)
  {
    fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params_set_access(playback_handle, p_hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
    fprintf(stderr, "cannot set access type (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params_set_format(playback_handle, p_hw_params, PCM_FORMAT)) < 0)
  {
    fprintf(stderr, "cannot set sample format (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, p_hw_params, &SAMPLE_RATE_p, 0)) < 0)
  {
    fprintf(stderr, "cannot set sample rate (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params_set_channels(playback_handle, p_hw_params, NUM_CH)) < 0)
  {
    fprintf(stderr, "cannot set channel count (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  if ((err = snd_pcm_hw_params(playback_handle, p_hw_params)) < 0)
  {
    fprintf(stderr, "cannot set parameters (%s)\n",
            snd_strerror(err));
    exit(1);
  }

  snd_pcm_hw_params_free(p_hw_params);

  if ((err = snd_pcm_prepare(playback_handle)) < 0)
  {
    fprintf(stderr, "cannot prepare audio play interface for use (%s)\n",
            snd_strerror(err));
    exit(1);
  }
//=====================================================================
#endif

  buffer = malloc(BUFF_LEN * (snd_pcm_format_width(PCM_FORMAT) / 8) * NUM_CH);
  frm = (short *)buffer;
  fprintf(stdout, "buffer allocated\n");

  while (1)
  {
#if UAC_DEBUG == 0
    if ((err = snd_pcm_readi(capture_handle, buffer, BUFF_LEN)) != BUFF_LEN)
    {
      fprintf(stderr, "read from audio interface failed (%s)\n",
              err, snd_strerror(err));
      exit(1);
    }
#endif

    //------Audio Processing Begins!
#if 0
    for (i = 0; i < BUFF_LEN * NUM_CH / 2; i++)
      frm[2 * i] >>= 4; //scale down only left channel
#endif
    //Write some testvalues for evaluation
#if UAC_DEBUG == 1
    for (i = 0; i < BUFF_LEN * NUM_CH / 2; i++)
    {
      frm[2 * i] = 5000;
      frm[2 * i + 1] = 32000;
    }
#endif

    //----Give some timesleep to measure complexity upperbound
    //usleep(10000); //10ms @ 4096/48000


    //------------------------------
#if REC_LOCAL == 0
    if ((err = snd_pcm_writei(playback_handle, buffer, BUFF_LEN)) != BUFF_LEN)
    {
      fprintf(stderr, "write to audio interface failed (%s)\n",
              err, snd_strerror(err));
      exit(1);
    }
#endif

#if REC_LOCAL
    //fprintf(stdout, "read %d done\n", i);
    fwrite(buffer, (BUFF_LEN * 2), sizeof(short), sig_FOut);
#endif
    //fprintf(stdout, "Sending pcm to Host...\r");
  }

  free(buffer);
  fprintf(stdout, "buffer freed\n");

  snd_pcm_close(capture_handle);
  fprintf(stdout, "audio capture interface closed\n");

  snd_pcm_close(playback_handle);
  fprintf(stdout, "audio playback interface closed\n");

  exit(0);
}
