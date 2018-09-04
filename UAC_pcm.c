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
	      
//Define Audio Parameters
#define BUFF_LEN 4096
#define NUM_CH 2
#define SAMPLE_RATE 48000
		  
		  
main (int argc, char *argv[])
{
  int i;
  int err;
  char *buffer;

  //FILE *sig_FOut;


  //Preparing sound capture device (physical)========
  snd_pcm_t *capture_handle;
  snd_pcm_hw_params_t *c_hw_params;
	snd_pcm_format_t format = snd_pcm_FORMAT_S16_LE;
	
  if ((err = snd_pcm_open (&capture_handle, argv[1], snd_pcm_STREAM_CAPTURE, 0)) < 0) {
    fprintf (stderr, "cannot open audio capture device %s (%s)\n", 
             argv[1],
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "audio interface opened\n");
/*
  if ((sig_FOut = fopen(argv[3], "wb")) == NULL)
  {
  	printf("cannot open audio file for recording\n");
  	exit (1);
  } 
  fprintf(stdout, "audio file for recording opened\n");
*/
  if ((err = snd_pcm_hw_params_malloc (&c_hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params allocated\n");
				 
  if ((err = snd_pcm_hw_params_any (capture_handle, c_hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params initialized\n");
	
  if ((err = snd_pcm_hw_params_set_access (capture_handle, c_hw_params, snd_pcm_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params access setted\n");
	
  if ((err = snd_pcm_hw_params_set_format (capture_handle, c_hw_params, format)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params format setted\n");
	
  if ((err = snd_pcm_hw_params_set_SAMPLE_RATE_near (capture_handle, c_hw_params, &SAMPLE_RATE, 0)) < 0) {
    fprintf (stderr, "cannot set sample SAMPLE_RATE (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params SAMPLE_RATE setted\n");

  if ((err = snd_pcm_hw_params_set_channels (capture_handle, c_hw_params, NUM_CH)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params channels setted\n");
	
  if ((err = snd_pcm_hw_params (capture_handle, c_hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "c_hw_params setted\n");
	
  snd_pcm_hw_params_free (c_hw_params);
  fprintf(stdout, "c_hw_params freed\n");
	
  if ((err = snd_pcm_prepare (capture_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
    exit (1);
  }
  fprintf(stdout, "audio capture interface prepared\n");
//=====================================================================


  //Preparing sound capture device (UAC2)========
		snd_pcm_t *playback_handle;
		snd_pcm_hw_params_t *p_hw_params;
	
		if ((err = snd_pcm_open (&playback_handle, argv[2], SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
			fprintf (stderr, "cannot open audio play device %s (%s)\n", 
				 argv[2],
				 snd_strerror (err));
			exit (1);
		}
		   
		if ((err = snd_pcm_hw_params_malloc (&p_hw_params)) < 0) {
			fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
				 
		if ((err = snd_pcm_hw_params_any (playback_handle, p_hw_params)) < 0) {
			fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_access (playback_handle, p_hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			fprintf (stderr, "cannot set access type (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_format (playback_handle, p_hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
			fprintf (stderr, "cannot set sample format (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, p_hw_params, SAMPLE_RATE, 0)) < 0) {
			fprintf (stderr, "cannot set sample rate (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_channels (playback_handle, p_hw_params, NUM_CH)) < 0) {
			fprintf (stderr, "cannot set channel count (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params (playback_handle, p_hw_params)) < 0) {
			fprintf (stderr, "cannot set parameters (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		snd_pcm_hw_params_free (p_hw_params);
	
		if ((err = snd_pcm_prepare (playback_handle)) < 0) {
			fprintf (stderr, "cannot prepare audio play interface for use (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
//=====================================================================


  buffer = malloc(BUFF_LEN * (snd_pcm_format_width(format) >> 3) * NUM_CH);
  fprintf(stdout, "buffer allocated\n");

  while (1) 
  {
    if ((err = snd_pcm_readi (capture_handle, buffer, BUFF_LEN)) != BUFF_LEN || )
	  {
      fprintf (stderr, "read from audio interface failed (%s)\n",
               err, snd_strerror (err));
      exit (1);
    }
		if ((err = snd_pcm_writei (playback_handle, buf, BUFF_LEN)) != BUFF_LEN) 
    {
			fprintf (stderr, "write to audio interface failed (%s)\n",
					     err, snd_strerror (err));
				exit (1);
		}
	
    //fprintf(stdout, "read %d done\n", i);
	  //fwrite(buffer, (BUFF_LEN*2), sizeof(short), sig_FOut);
    fprintf(stdout, "Sending pcm to Host...\r");
  }


  free(buffer);






  fprintf(stdout, "buffer freed\n");
	

  snd_pcm_close (capture_handle);
  fprintf(stdout, "audio interface closed\n");






  exit (0);
}
