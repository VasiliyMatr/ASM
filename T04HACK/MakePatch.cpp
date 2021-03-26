
#include "MakePatch.h"

    /* TODO:
     * Need to make normal isBadPtr func for Linux OS
     */
    bool isBadPtr( const void* ptr );

/* file patch stuff here */
void fileChange( char* outBuffPtr );

/* function to get file length
 * 
 * !!! REWINDS FILE TO BEGIN !!!
 */
size_t getFileLength( FILE* filePtr );

/* menu */
patchError menu();

void clalcMandel( sf::Image& buff );

sf::Color getColor( char iters );

/* Main patching function */
patchError makePatch( const char* inFileName, const char* outFileName )
{
  /* patch exe menu */
    patchError menuError = menu();
    if (menuError != patchError::OK_)
        return menuError;

  /* input & output files names ptrs check */
    if (isBadPtr (inFileName) || isBadPtr (outFileName))
        return patchError::BAD_PTR_;

  /* opening input file */
    FILE* inFilePtr = fopen (inFileName, "rb");
    if (inFilePtr == nullptr)
        return patchError::FILE_ERROR_;

  /* getting num of bytes in input file */
    size_t bytesNum = getFileLength (inFilePtr);
    if (bytesNum == 0)
        return patchError::FILE_ERROR_;

  /* allocating memory for output buff - with patched file */
    char* outBuffPtr = (char* )calloc (sizeof (char), bytesNum);
    if (outBuffPtr == nullptr)
    {
        fclose (inFilePtr);
        return patchError::MEM_ERROR_;
    }

  /* reading info and closing input file */
    fread (outBuffPtr, sizeof (char), bytesNum, inFilePtr);
    fclose (inFilePtr);

  /* patching file */
    fileChange (outBuffPtr);

  /* opening output file */
    FILE* outFilePtr = fopen (outFileName, "wb");
    if (outFilePtr == nullptr)
        return patchError::FILE_ERROR_;

  /* writing data and closing output file */
    fwrite (outBuffPtr, sizeof (char), bytesNum, outFilePtr);
    fclose (outFilePtr);

  /* free outBuffPtr memory */
    free (outBuffPtr);

  /* all is good, patch finished */
    return patchError::OK_;
}

bool isBadPtr( const void* ptr )
{
    if (ptr == nullptr)
        return true;

    return false;
}

void fileChange( char* outBuffPtr )
{

  /* num of patch pastes */
    const size_t       PATCH_PASTES_NUMBER_                              = 3;
  /* addrs to add pastes */
    const size_t       PATCH_PASTES_POSES_  [PATCH_PASTES_NUMBER_]       = { 0x0000, 0x00F6, 0x015A };
  /* patch pastes to add to code */
    const char         PATCH_PASTES_        [PATCH_PASTES_NUMBER_][100]  = {

        "\xB0\xFE\xB4\x0A" "\x90\x90\x90\x90\x90\x90\x90\x90\x90" ,
        "bcdefghijk"                                              ,
        "HHHHHHHHHH"

     };

  /* pasting */
    for (size_t pasteID = 0; pasteID < PATCH_PASTES_NUMBER_; ++pasteID)
        memcpy (PATCH_PASTES_POSES_[pasteID] + outBuffPtr ,
                PATCH_PASTES_[pasteID]                    ,
                strlen (PATCH_PASTES_[pasteID]));
}

size_t getFileLength( FILE* filePtr )
{
    assert (!isBadPtr (filePtr));

  /* moving to file end */
    fseek (filePtr, 0, SEEK_END);
  /* getting num of bytes */
    int bytesNum = ftell (filePtr);
  /* rewinding file back to begin */
    rewind (filePtr);

    return bytesNum;
}

patchError menu()
{
  /* window sizes */
    const size_t WINDOW_WIDTH_  = 800;
    const size_t WINDOW_HEIGHT_ = 600;
  /* Creating main window */
    sf::RenderWindow window (sf::VideoMode (WINDOW_WIDTH_, WINDOW_HEIGHT_),
                             "VZLOM patch wizard v1.3.3.7.");

  /* Drawable picture load */
    sf::Image image1;
    image1.create (WINDOW_WIDTH_, WINDOW_HEIGHT_);
    sf::Image image2;
    if (!(image2.loadFromFile ("picks//aboba.png")))
        return patchError::FILE_ERROR_;

    clalcMandel (image1);

  /* Creating texture with picture */
    sf::Texture texture1;
    texture1.loadFromImage (image1);
  /* Creating sprite with picture */
    sf::Sprite sprite1;
    sprite1.setTexture (texture1);

  /* Creating texture with picture */
    sf::Texture texture2;
    texture2.loadFromImage (image2);
  /* Creating sprite with picture */
    sf::Sprite sprite2;
    sprite2.setTexture (texture2);
    sprite2.move (309, 254);    

  /* Main cycle */
    while (window.isOpen ())
    {
        /* Events processing */
        sf::Event event;
        while (window.pollEvent (event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close ();
                    break;

                case sf::Event::MouseButtonPressed:

                  /* for mouse coords */
                    const sf::Vector2i leftTopPos = sf::Vector2i (309, 254);
                    const sf::Vector2i righBotPos = sf::Vector2i (490, 300);
                  /* getting mouse coord */
                    sf::Vector2i mousePos = sf::Mouse::getPosition (window);
                  /* button check */
                    if (mousePos.x >= leftTopPos.x && mousePos.x <= righBotPos.x &&
                        mousePos.y >= leftTopPos.y && mousePos.y <= righBotPos.y)
                        {
                            system ("google-chrome --new-window https:////www.youtube.com//watch?v=YRbpAIn_dg4");
                            window.close ();
                        }

                    break;
            }
        }

        /* Clearing window */
        window.clear ();
        /* Draw funcs here */
        window.draw (sprite1);
        /* Draw funcs here */
        window.draw (sprite2);
        /* Displaying result */
        window.display ();
    }

    return patchError::OK_;
}

/* window sizes */
const size_t L_ = 0;
const size_t R_ = 800;

const size_t T_ = 0;
const size_t B_ = 600;

const size_t WINDOW_WIDTH_  = R_ - L_;
const size_t WINDOW_HEIGHT_ = B_ - T_;

/* Max number of counts in cycle */
    const size_t MAX_ITERATIONS_NUM_ = 0xFF;

/* Axis limits */
    const float  X_LIMITS_RIGH_   =  1.0;
    const float  X_LIMITS_LEFT_   = -1.3;
    const float  Y_LIMITS_TOP_    =  1.0;
    const float  Y_LIMITS_BOT_    = -1.0;

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
              #define SET_COLOR_( PIXEL_ID )                            \
              {                                                         \
              int iters = _mm_cvtsi128_si32 (_mm_shuffle_epi32          \
              (count, _MM_SHUFFLE (0, 0, 0, PIXEL_ID)));                \
              if (iters > 10)                                           \
              buff.setPixel (reSteps + PIXEL_ID + L_, imSteps + T_,     \
              getColor (iters));                                        \
              }

              SET_COLOR_ (0) SET_COLOR_ (1) SET_COLOR_ (2) SET_COLOR_ (3) 

              #undef SET_COLOR_
        }
    }
}

sf::Color getColor( char iters )
{
    return sf::Color (255 - iters / 2%2 * 255, 255 - iters / 2%2 * 255, 255 - iters / 2%2 * 255, 255 - iters);
    // (-cosh (iters) * iters, 256 - sin (iters) * 256, iters, sin (iters) * sin (iters) * sin (iters) * sin (iters) * iters * 16 );
}