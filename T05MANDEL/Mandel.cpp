
#include "Mandel.h"


/* window sizes */
const size_t WINDOW_WIDTH_  = 800;
const size_t WINDOW_HEIGHT_ = 600;


void clalcMandel( sf::Image& buff );

sf::Color getColor( char iters );

void mandelDraw()
{
  /* Creating main window */
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH_, WINDOW_HEIGHT_),
                             "A B O B A");

  /* creating image */
    sf::Image buffer;
    buffer.create (WINDOW_WIDTH_, WINDOW_HEIGHT_);
  /* creating texture */
    sf::Texture texture;
    texture.loadFromImage (buffer);
  /* Creating sprite with texture */
    sf::Sprite sprite (texture);   

    size_t   fpsCnt = 0;
    clock_t  time   = clock ();

  /* Main cycle */
    while (window.isOpen ())
    {
        float fps = (fpsCnt++) * CLOCKS_PER_SEC / ((float)clock () - time);
        printf ("%f" "\n", fps);

        /* Events processing */
        sf::Event event;
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                    window.close ();
        }

        /* Clearing window */
        window.clear ();
        /* Draw funcs here */
            clalcMandel (buffer);

            texture.loadFromImage (buffer);
            window.draw (sprite);
        /* Displaying result */
        window.display ();
    }
}

/* Max number of counts in cycle */
    const size_t MAX_ITERATIONS_NUM_ = 0xFF;

/* Axis limits */
    const float  X_LIMITS_RIGH_   =  3.0;
    const float  X_LIMITS_LEFT_   = -3.0;
    const float  Y_LIMITS_TOP_    =  3.0;
    const float  Y_LIMITS_BOT_    = -3.0;

/* Target Radius */
    const int32_t MAX_RAD_SQ_ = 0xFF;
    const float  imOneStepShift = (Y_LIMITS_TOP_  - Y_LIMITS_BOT_)  / WINDOW_HEIGHT_;
    const float  reOneStepShift = (X_LIMITS_RIGH_ - X_LIMITS_LEFT_) / WINDOW_WIDTH_;

void clalcMandel( sf::Image& buff )
{
  /* fast access consts */
    const __m128i MAX_RADS_ = _mm_set1_epi32 (MAX_RAD_SQ_);
    const __m128i MASK_     = _mm_set1_epi8 (0xFF);

    for (size_t imSteps = 0; imSteps < WINDOW_HEIGHT_; ++imSteps)
    {
          /* counting imagined coordinate */
            float imVal = Y_LIMITS_BOT_ + imSteps * imOneStepShift;
        
          /* imagined coordinates vector */
            __m128 im = _mm_set1_ps (imVal);

        for (size_t reSteps = 0; reSteps < WINDOW_WIDTH_; reSteps += 4)
        {
          /* real coordinates vector */
            __m128 re = _mm_set_ps (X_LIMITS_LEFT_  + (reSteps + 3) * reOneStepShift,
                                    X_LIMITS_LEFT_  + (reSteps + 2) * reOneStepShift,
                                    X_LIMITS_LEFT_  + (reSteps + 1) * reOneStepShift,
                                    X_LIMITS_LEFT_  + (reSteps + 0) * reOneStepShift );

          /* iterations counters vector */
            __m128i count = _mm_set1_epi64x (0);

          /* vectors of countable values */
            __m128  transIm = im;
            __m128  transRe = re;

          /* to check bounds cross */
            __m128i ctrlMask = _mm_set1_epi64x (0xFFFFFFFFffffffff);

            for (register size_t iterNum = 0; iterNum < MAX_ITERATIONS_NUM_; ++iterNum)
            {
              /* im coordinates squares vaules vectors */
                __m128  imSq    = _mm_mul_ps (transIm, transIm);

              /* next vals */ transIm = _mm_add_ps (transIm, transIm);
              /* next vals */ transIm = _mm_mul_ps (transIm, transRe);
              
              /* re coordinates squares vaules vectors */
                __m128  reSq    = _mm_mul_ps (transRe, transRe);

              /* integer radius */
                __m128i radInt  = _mm_cvtps_epi32 (_mm_add_ps (imSq, reSq));

              /* next vals */ transRe = _mm_sub_ps (reSq, imSq);
              /* next vals */ transRe = _mm_add_ps (transRe, re);

              /* comparations vector */
                __m128i cmpMask = _mm_cmplt_epi32 (radInt, MAX_RADS_);

              /* bounds cross vector */
                ctrlMask = _mm_and_si128 (ctrlMask, cmpMask);

              /* adding values */
                count = _mm_sub_epi32 (count, ctrlMask);

              /* next vals */ transIm = _mm_add_ps (transIm, im);

                if (ctrlMask[0] == 0 && ctrlMask[1] == 0)
                    break;
            }

            /* TODO: rm this shittt & make normal */
              #define SET_COLOR_( PIXEL_ID )                      \
              buff.setPixel (reSteps + PIXEL_ID, imSteps,         \
              getColor (_mm_cvtsi128_si32 (_mm_shuffle_epi32      \
              (count, _MM_SHUFFLE (0, 0, 0, PIXEL_ID)))));

              SET_COLOR_ (0) SET_COLOR_ (1) SET_COLOR_ (2) SET_COLOR_ (3) 

              #undef SET_COLOR_
        }
    }
}

sf::Color getColor( char iters )
{
    return sf::Color (iters, iters, iters, 255);
    // (-cosh (iters) * iters, 256 - sin (iters) * 256, iters, sin (iters) * sin (iters) * sin (iters) * sin (iters) * iters * 16 );
}