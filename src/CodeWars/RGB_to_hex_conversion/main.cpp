// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - RGB to hex conversion                                  ║
// ║ File         : main.cpp                                                          ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/08/21                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝
#include <iostream>
#include <string>

/**
 * Takes in a decimal number n [0; 255] and converts to hex.
 * Will add one leading zero if n = 1.
 *
 * @param n Decimal to be converted.
 * @return n as hex. 00 if n <= 0, FF if n >= 255
 */
std::string decimalToHex(int n) {
  // Basecases
  if (n <= 0) return "00";
  if (n >= 255) return "FF";

  // Calculate hex
  constexpr char hexValues[] = {"0123456789ABCDEF"};
  std::string res;
  // Division method
  while (n > 0) {
    res = hexValues[n % 16] + res;
    n /= 16;
  }
  // Add leading zero if required
  if (res.length() == 1) res.insert(0, 1, '0');
  return res;
}

// Entrypoint
std::string rgb_to_hex(int r, int g, int b)
{
  return decimalToHex(r) + decimalToHex(g) + decimalToHex(b);
}


int main() {

  std::cout << "(0, 0, 0) should be #000000 and is #" << rgb_to_hex(  0,  0,  0) << std::endl;
  std::cout << "(1, 2, 3) should be #010203 and is #" << rgb_to_hex(  1,  2,  3) << std::endl;
  std::cout << "(255, 255, 255) should be #FFFFFF and is #" << rgb_to_hex(255,255,255) << std::endl;
  std::cout << "(254, 253, 252) should be #FEFDFC and is #" << rgb_to_hex(254,253,252) << std::endl;
  std::cout << "(-20, 275, 125) should be #00FF7D and is #" << rgb_to_hex(-20,275,125) << std::endl;
  return 0;
}
