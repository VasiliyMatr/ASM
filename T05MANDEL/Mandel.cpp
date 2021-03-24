
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
    const float MAX_RAD_SQ_ = 1000.0;

void clalcMandel( sf::Image& buff )
{
    const __m128 max_rad = { MAX_RAD_SQ_, MAX_RAD_SQ_, MAX_RAD_SQ_, MAX_RAD_SQ_ };

    for (size_t y = 0; y < WINDOW_HEIGHT_; ++y)
    {
            float yOneStepShift = (Y_LIMITS_TOP_  - Y_LIMITS_BOT_)  / WINDOW_HEIGHT_;
            float im_ft_val = Y_LIMITS_BOT_  + (float) y * yOneStepShift;
        
            __m128 im = { im_ft_val ,
                          im_ft_val ,
                          im_ft_val ,
                          im_ft_val  };

        for (size_t x = 0; x < WINDOW_WIDTH_; x += 4)
        {
            float xOneStepShift = (X_LIMITS_RIGH_ - X_LIMITS_LEFT_) / WINDOW_WIDTH_;
            float re_ft_val = X_LIMITS_LEFT_  + (float) x * xOneStepShift;
        
            __m128  re = {  re_ft_val                       ,
                            re_ft_val + xOneStepShift       ,
                            re_ft_val + xOneStepShift * 2   ,
                            re_ft_val + xOneStepShift * 3    };

            __m128i counts = { 0, 0 };

            __m128  transIm = im;
            __m128  transRe = re;

            for (register size_t cnt = 0; cnt < 0xFF; cnt++)
            {
                __m128  imSq = _mm_mul_ps (transIm, transIm);
                __m128  reSq = _mm_mul_ps (transRe, transRe);
                
                __m128  rad  = _mm_add_ps (imSq, reSq);
                __m128  diff = _mm_cmple_ps (rad, max_rad);
                
                if (diff[0] != diff[0])
                    ++counts[0];
                if (diff[1] != diff[1])
                    counts[0] += (uint64_t)1 << 32;
                if (diff[2] != diff[2])
                    ++counts[1];
                if (diff[3] != diff[3])
                    counts[1] += (uint64_t)1 << 32;

                if (diff[0] == 0 && diff[1] == 0 && diff[2] == 0 && diff[3] == 0)
                    break;

                transIm = _mm_add_ps (transIm, transIm);
                transIm = _mm_mul_ps (transIm, transRe);
                transIm = _mm_add_ps (transIm, im);

                transRe = _mm_sub_ps (reSq, imSq);
                transRe = _mm_add_ps (transRe, re);
            }

            buff.setPixel (x    , y    , getColor (counts[0] & 0xFF));
            buff.setPixel (x + 1, y    , getColor (counts[0] >> 32));
            buff.setPixel (x + 2, y    , getColor (counts[1] & 0xFF));
            buff.setPixel (x + 3, y    , getColor (counts[1] >> 32)); 
        }
    }
}

sf::Color getColor( char iters )
{
    return sf::Color
    (-sin (iters) * sin (iters) * 256, 256 - atan (iters) * 256, iters, 255);
}