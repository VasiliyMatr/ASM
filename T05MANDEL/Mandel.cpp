
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
        float fps = (fpsCnt++) * CLOCKS_PER_SEC / (clock () - time);
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

const size_t MAX_ITERATIONS_NUM_ = 255;
const float  X_LIMITS_   = 1.;
const float  Y_LIMITS_   = 1.; 
const float  MAX_RAD_SQ_ = 1000.;

void clalcMandel( sf::Image& buff )
{
    for (size_t x = 0; x < WINDOW_WIDTH_; ++x)
    {
        float re = ((float)x / WINDOW_WIDTH_ - 0.5) * X_LIMITS_ * 2;

        for (size_t y = 0; y < WINDOW_HEIGHT_; ++y)
        {
            size_t iters = 0;

            float im = ((float)y / WINDOW_HEIGHT_ - 0.5) * Y_LIMITS_ * 2;

            float transIm = im;
            float transRe = re;

            for (; iters < MAX_ITERATIONS_NUM_; ++iters)
            {
                if (transRe * transRe + transIm * transIm > MAX_RAD_SQ_)
                    break;

                float newIm = 2 * transIm * transRe + im;
                transRe = transRe * transRe - transIm * transIm + re;
                transIm = newIm;
            }

            buff.setPixel (x, y, getColor (iters));
        }
    }
}

sf::Color getColor( char iters )
{
    return sf::Color
    (-sin (iters) * sin (iters) * 256, 256 - atan (iters) * 256, iters, 255);
}