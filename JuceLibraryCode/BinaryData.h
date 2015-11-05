/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_42430924_INCLUDED
#define BINARYDATA_H_42430924_INCLUDED

namespace BinaryData
{
    extern const char*   multirow_png;
    const int            multirow_pngSize = 284;

    extern const char*   one_row_png;
    const int            one_row_pngSize = 250;

    extern const char*   error_png;
    const int            error_pngSize = 873;

    extern const char*   refresh_png;
    const int            refresh_pngSize = 978;

    extern const char*   instrument_png;
    const int            instrument_pngSize = 412;

    extern const char*   options_cog_png;
    const int            options_cog_pngSize = 1082;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 6;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
