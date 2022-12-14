/*****************************************************************************
 *                                                                           *
 *                                 V E S A                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    VESA-Treiber ueber 16-Bit BIOS.                          *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 18.3.2017                       *
 *****************************************************************************/

#include "devices/VESA.h"
#include "kernel/BIOS.h"
#include "kernel/Globals.h"

// Informationen ueber einen VESA-Grafikmodus
// (siehe http://wiki.osdev.org/VESA_Video_Modes)
struct VbeModeInfoBlock {
    unsigned short attributes;
    unsigned char winA, winB;
    unsigned short granularity;
    unsigned short winsize;
    unsigned short segmentA, segmentB;
    unsigned short realFctPtr[2];
    unsigned short pitch;  // Bytes pro Scanline

    unsigned short Xres, Yres;
    unsigned char Wchar, Ychar, planes, bpp, banks;
    unsigned char memory_model, bank_size, image_pages;
    unsigned char reserved0;

    unsigned char red_mask, red_position;
    unsigned char green_mask, green_position;
    unsigned char blue_mask, blue_position;
    unsigned char rsv_mask, rsv_position;
    unsigned char directcolor_attributes;

    unsigned int physbase;  // Adresse des Linear-Framebuffers
    unsigned int OffScreenMemOffset;
    unsigned short OffScreenMemSize;
} __attribute__((packed));

// Informationen ueber die Grafikkarte
// (siehe http://wiki.osdev.org/VESA_Video_Modes)
struct VbeInfoBlock {
    char VbeSignature[4];            // == "VESA"
    unsigned short VbeVersion;       // == 0x0300 for VBE 3.0
    unsigned short OemStringPtr[2];  // isa vbeFarPtr
    unsigned char Capabilities[4];
    unsigned short VideoModePtr[2];  // isa vbeFarPtr
    unsigned short TotalMemory;      // as # of 64KB blocks
} __attribute__((packed));

/*****************************************************************************
 * Methode:         VESA::initTextMode                                       *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Schalter in den Text-Modus 80x25 Zeichen.                *
 *****************************************************************************/
void VESA::initTextMode() {
    BC_params->AX = 0x4f02;      // SVFA BIOS, init mode
    BC_params->BX = 0x4003;      // 80x25
    BIOS::Int(0x10);
}

/*****************************************************************************
 * Methode:         VESA::initGraphicMode                                    *
 *---------------------------------------------------------------------------*
 * Parameter:       Nummer des Grafikmodus (siehe VESA.h)                    *
 *                                                                           *
 * Beschreibung:    Bestimmten Grafikmodus einschalten. Dies wird durch      *
 *                  einen Aufruf des BIOS gemacht.                           *
 *****************************************************************************/
bool VESA::initGraphicMode(unsigned short mode) {

    // Alle Grafikmodi abfragen
    BC_params->AX = 0x4F00;
    BC_params->ES = RETURN_MEM >> 4;
    BC_params->DI = RETURN_MEM & 0xF;
    BIOS::Int(0x10);

    VbeInfoBlock* ib = reinterpret_cast<VbeInfoBlock*>(RETURN_MEM);

    // Signaturen pruefen
    if (BC_params->AX != 0x004F) {
        log.error() << "VESA wird nicht unterstuetzt." << endl;
        return false;
    }
    if (ib->VbeSignature[0] != 'V' || ib->VbeSignature[1] != 'E' ||
        ib->VbeSignature[2] != 'S' || ib->VbeSignature[3] != 'A') {
        log.error() << "VESA wird nicht unterstuetzt." << endl;
        return false;
    }

    //    kout << "TotalVideoMemory:  " << ((ib->TotalMemory*65536) / (1024*1024)) << " MB" << endl;

    // Gewuenschten Grafikmodus aus Antwort suchen
    unsigned short* modePtr = reinterpret_cast<unsigned short*>((ib->VideoModePtr[1] << 4) + ib->VideoModePtr[0]);
    for (int i = 0; modePtr[i] != 0xFFFF; ++i) {
        // Gewuenschter Grafikmodus gefunden?
        if (modePtr[i] == mode) {
            VbeModeInfoBlock* minf = reinterpret_cast<VbeModeInfoBlock*>(RETURN_MEM);

            // Weitere Infos ueber diesen Grafikmodus abfragen
            BC_params->AX = 0x4F01;
            BC_params->CX = mode;
            BC_params->ES = RETURN_MEM >> 4;
            BC_params->DI = RETURN_MEM & 0xF;
            BIOS::Int(0x10);

            // Text-Modi 0-3 haben keinen LFB
            if (mode > 3 && (minf->attributes & 0x90) == 0) {
                log.error() << "Grafikmodus bietet keinen linearen Framebuffer." << endl;
                return false;
            }

            mode_nr = mode;
            xres = minf->Xres;
            yres = minf->Yres;
            bpp = static_cast<int>(minf->bpp);
            lfb = minf->physbase;

            hfb = reinterpret_cast<unsigned int>(new char[xres * yres * bpp / 8]);

            // Grafikmodus einschalten
            BC_params->AX = 0x4f02;  // SVFA BIOS, init mode
            BC_params->BX = mode;
            BIOS::Int(0x10);
            return true;
        }
    }
    log.error() << "Grafikmodus nicht gefunden." << endl;
    return false;
}
