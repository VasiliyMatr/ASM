
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
    const size_t MAX_ITERATIONS_NUM_ = 255;

/* Axis limits */
    const float  X_LIMITS_RIGH_   = +1.;
    const float  X_LIMITS_LEFT_   = -1.;
    const float  Y_LIMITS_TOP_   = +1.;
    const float  Y_LIMITS_BOT_   = -1.;

/* Target Radius */
    const int32_t MAX_RAD_SQ_ = 100000;

void clalcMandel( sf::Image& buff )
{
    __m128 MAX_RADS_ = _mm_set1_ps (MAX_RAD_SQ_);
    __m128 ones = _mm_set1_ps (1);
    __m128i MASK_ = _mm_set1_epi8 (0xFF);

    for (size_t y = 0; y < WINDOW_HEIGHT_; ++y)
    {
            float imVal = Y_LIMITS_BOT_ + (float) y * (Y_LIMITS_TOP_ - Y_LIMITS_BOT_) / WINDOW_HEIGHT_;
        
            __m128 im = _mm_set1_ps (imVal);

        for (size_t x = 0; x < WINDOW_WIDTH_; x += 4)
        {
            float xOneStepShift = (X_LIMITS_RIGH_ - X_LIMITS_LEFT_) / WINDOW_WIDTH_;

            __m128 re = _mm_set_ps (X_LIMITS_LEFT_  + (x + 3) * xOneStepShift,
                                    X_LIMITS_LEFT_  + (x + 2) * xOneStepShift,
                                    X_LIMITS_LEFT_  + (x + 1) * xOneStepShift,
                                    X_LIMITS_LEFT_  + (x + 0) * xOneStepShift );

            int  counts [4] = {0, 0, 0, 0};

            __m128i cont = _mm_set1_epi32 (0);

            __m128  transIm = im;
            __m128  transRe = re;

            __m128i b = _mm_set1_epi32 (1);

            for (int cnt = 0; cnt < 0xFF; ++cnt)
            {
                __m128  imSq = _mm_mul_ps (transIm, transIm);
                __m128  reSq = _mm_mul_ps (transRe, transRe);

                __m128  cmp  = _mm_cmple_ps (MAX_RADS_, _mm_add_ps (imSq, reSq));

                __m128i a = _mm_cvtps_epi32 (_mm_add_ps (cmp, ones));

                b = _mm_and_si128 (a, b);

                cont = _mm_add_epi32 (cont, b);

                if (_mm_test_all_zeros (MASK_, b))
                    break;

                // if (*((int64_t*)&cmp + 0) != 0 || *((int64_t*)&cmp + 1) != 0)
                //     break;

                transIm = _mm_add_ps (transIm, transIm);
                transIm = _mm_mul_ps (transIm, transRe);
                transIm = _mm_add_ps (transIm, im);
                transRe = _mm_sub_ps (reSq, imSq);
                transRe = _mm_add_ps (transRe, re);
            }

            counts[0] = _mm_cvtsi128_si32 (cont);

            buff.setPixel (x    , y    , getColor (counts[0]));
            buff.setPixel (x + 1, y    , getColor (counts[1]));
            buff.setPixel (x + 2, y    , getColor (counts[2]));
            buff.setPixel (x + 3, y    , getColor (counts[3])); 
        }
    }
}

sf::Color getColor( char iters )
{
    return sf::Color (iters, iters, iters, 255);
    // (-sin (iters) * sin (iters) * 256, 256 - atan (iters) * 256, iters, 255);
}