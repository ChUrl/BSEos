/*****************************************************************************
 *                                                                           *
 *                                K E Y                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Taste, bestehend aus ASCII-, Scan-Code und Modifier-Bits.*
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/
#ifndef Key_include__
#define Key_include__

class Key {
    // Kopieren erlaubt!

    unsigned char asc;   // ASCII code
    unsigned char scan;  // scan code
    unsigned char modi;  // modifier

    // Bit-Masken fuer die Modifier-Tasten
    struct mbit {
        enum {
            shift = 1,
            alt_left = 2,
            alt_right = 4,
            ctrl_left = 8,
            ctrl_right = 16,
            caps_lock = 32,
            num_lock = 64,
            scroll_lock = 128
        };
    };

public:
    // DEFAULT-KONSTRUKTOR: setzt ASCII, Scancode und Modifier auf 0
    //                      und bezeichnet so einen ungueltigen Tastencode
    Key() : asc(0), scan(0), modi(0) {}

    // VALID: mit Scancode = 0 werden ungueltige Tasten gekennzeichnet.
    bool valid() const { return scan != 0; }

    // INVALIDATE: setzt den Scancode auf Null und sorgt somit fuer einen
    //             ungueltigen Tastencode.
    void invalidate() { scan = 0; }

    // ASCII, SCANCODE: Setzen und Abfragen von Ascii und Scancode
    void ascii(unsigned char a) { asc = a; }
    void scancode(unsigned char s) { scan = s; }
    unsigned char ascii() const { return asc; }
    unsigned char scancode() const { return scan; }

    //
    // Funktionen zum Setzen und Loeschen von SHIFT, ALT, CTRL usw.
    //
    void shift(bool pressed) {
        modi = pressed ? modi | mbit::shift : modi & ~mbit::shift;
    }

    void alt_left(bool pressed) {
        modi = pressed ? modi | mbit::alt_left : modi & ~mbit::alt_left;
    }

    void alt_right(bool pressed) {
        modi = pressed ? modi | mbit::alt_right : modi & ~mbit::alt_right;
    }

    void ctrl_left(bool pressed) {
        modi = pressed ? modi | mbit::ctrl_left : modi & ~mbit::ctrl_left;
    }

    void ctrl_right(bool pressed) {
        modi = pressed ? modi | mbit::ctrl_right : modi & ~mbit::ctrl_right;
    }

    void caps_lock(bool pressed) {
        modi = pressed ? modi | mbit::caps_lock : modi & ~mbit::caps_lock;
    }

    void num_lock(bool pressed) {
        modi = pressed ? modi | mbit::num_lock : modi & ~mbit::num_lock;
    }

    void scroll_lock(bool pressed) {
        modi = pressed ? modi | mbit::scroll_lock : modi & ~mbit::scroll_lock;
    }

    //
    // Funktionen zum Abfragen von SHIFT, ALT, CTRL usw.
    //
    bool shift() const { return (modi & mbit::shift) != 0; }
    bool alt_left() const { return (modi & mbit::alt_left) != 0; }
    bool alt_right() const { return (modi & mbit::alt_right) != 0; }
    bool ctrl_left() const { return (modi & mbit::ctrl_left) != 0; }
    bool ctrl_right() const { return (modi & mbit::ctrl_right) != 0; }
    bool caps_lock() const { return (modi & mbit::caps_lock) != 0; }
    bool num_lock() const { return (modi & mbit::num_lock) != 0; }
    bool scroll_lock() const { return (modi & mbit::scroll_lock) != 0; }
    bool alt() const { return alt_left() || alt_right(); }
    bool ctrl() const { return ctrl_left() || ctrl_right(); }

    operator char() const { return static_cast<char>(asc); }

    // Scan-Codes einiger spezieller Tasten
    struct scan {
        enum {
            f1 = 0x3b,
            del = 0x53,
            up = 72,
            down = 80,
            left = 75,
            right = 77,
            div = 8
        };
    };
};

#endif
