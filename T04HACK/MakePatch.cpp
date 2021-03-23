
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
    sf::Image image;
    if (!(image.loadFromFile ("picks//lmao.jpg")))
        return patchError::FILE_ERROR_;
  /* Creating texture with picture */
    sf::Texture texture;
    texture.loadFromImage (image);
  /* Creating sprite with picture */
    sf::Sprite sprite;
    sprite.setTexture (texture);   

  /* time counter */


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
                    const sf::Vector2i leftTopPos = sf::Vector2i (315, 465);
                    const sf::Vector2i righBotPos = sf::Vector2i (480, 500);
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
        window.draw (sprite);
        /* Displaying result */
        window.display ();
    }

    return patchError::OK_;
}