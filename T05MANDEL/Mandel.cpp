
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
{;
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
            char outFlag[4] = {1, 1, 1, 1};

            __m128  transIm = im;
            __m128  transRe = re;

            for (int cnt = 0; cnt < 0xFF; ++cnt)
            {
                __m128  imSq = _mm_mul_ps (transIm, transIm);
                __m128  reSq = _mm_mul_ps (transRe, transRe);

                __m128  rad = _mm_add_ps (imSq, reSq);

                float   rads[4] = { _mm_cvtss_f32(_mm_shuffle_ps(rad, rad, _MM_SHUFFLE(0, 0, 0, 0))),
                                    _mm_cvtss_f32(_mm_shuffle_ps(rad, rad, _MM_SHUFFLE(0, 0, 0, 1))),
                                    _mm_cvtss_f32(_mm_shuffle_ps(rad, rad, _MM_SHUFFLE(0, 0, 0, 2))),
                                    _mm_cvtss_f32(_mm_shuffle_ps(rad, rad, _MM_SHUFFLE(0, 0, 0, 3)))};

                for (size_t radId = 0; radId < 4; radId++)
                    if (rads[radId] >= MAX_RAD_SQ_) outFlag[radId] = 0;

                for (size_t cntId = 0; cntId < 4; cntId++)
                    counts[cntId] += outFlag[cntId];

                if (!*(int32_t*)outFlag)
                    break;

                transIm = _mm_add_ps (transIm, transIm);
                transIm = _mm_mul_ps (transIm, transRe);
                transIm = _mm_add_ps (transIm, im);
                transRe = _mm_sub_ps (reSq, imSq);
                transRe = _mm_add_ps (transRe, re);
            }

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