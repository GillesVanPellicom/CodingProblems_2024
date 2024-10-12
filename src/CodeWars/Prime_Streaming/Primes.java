// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Project      : CodeWars - Prime Sieve using BitSet                               ║
// ║ Version      : v0.1.0 "initial implementation"                                   ║
// ║ File         : Primes.java                                                       ║
// ║ Author(s)    : Gilles Van pellicom                                               ║
// ║ Date         : 2024/10/12                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

import java.util.BitSet; // Importing BitSet for efficient prime number storage
import java.util.stream.IntStream; // Importing IntStream for functional-style operations

public class Primes {
    // Magic numbers
    public static final int MAX_PRIME = 850_000_000; // Maximum prime number to calculate
    public static final int SQRT_MAX_PRIME = (int) Math.sqrt(MAX_PRIME); // Square root of the maximum prime
    public static final BitSet bitset = new PrimeBitSet().getBitSet(); // Initialize prime bitset

    /**
     * Returns an IntStream of prime numbers starting from 2.
     * Utilizes the bitset to find the next set bit (prime).
     * 
     * @return IntStream of prime numbers
     */
    public static IntStream stream() {
        return IntStream.iterate(2, i -> bitset.nextSetBit(i + 1)); // Iterate through primes
    }
}

class PrimeBitSet {
    // BitSet to hold prime status of numbers
    private final BitSet bitset;

    public PrimeBitSet() {
        bitset = new BitSet(Primes.MAX_PRIME); // Create a BitSet with size MAX_PRIME
        bitset.set(2, Primes.MAX_PRIME); // Set bits for all numbers starting from 2

        // Implement Sieve of Eratosthenes to mark non-prime numbers
        for (int i = 2; i * i < Primes.MAX_PRIME; i = bitset.nextSetBit(i + 1)) {
            if (bitset.get(i)) { // If `i` is prime
                for (int j = i * i; j < Primes.MAX_PRIME; j += i) { // Clear multiples of `i`
                    bitset.clear(j); // Mark non-prime
                }
            }
        }
    }

    /**
     * Returns the BitSet containing the prime numbers.
     * 
     * @return BitSet with prime indicators
     */
    public BitSet getBitSet() {
        return bitset; // Provide access to the internal BitSet
    }
}
