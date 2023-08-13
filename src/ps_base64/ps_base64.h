//
//  base64 encoding and decoding with C++.
//  Version: 2.rc.09 (release candidate)
//

/**
 * @brief Modifed by Nicholas Watt (github.com/BurgerRelish)
 */

#ifndef PS_BASE64_H
#define PS_BASE64_H

#include <ps_stl.h>

ps::string base64_encode     (ps::string const& s, bool url = false);
ps::string base64_encode_pem (ps::string const& s);
ps::string base64_encode_mime(ps::string const& s);

ps::string base64_decode(ps::string const& s, bool remove_linebreaks = false);
ps::string base64_encode(unsigned char const*, size_t len, bool url = false);

#endif