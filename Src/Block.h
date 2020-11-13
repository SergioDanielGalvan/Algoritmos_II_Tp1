//Archivo fuente header clase Block / AlgoBlock del tp0 para la materia 9512 Algoritmos y Programación 2.

#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdlib>
#include <string>
#include "lista.h"
#include "TiposHash.h"
#include "Transaction.h"

#include "BlockChainDataTypes.h"

// const size_t LargoHashEstandar = 64;
// const size_t LargoHashFirma    = 40;	// Hash Pública de la Cuenta
// https://stackoverflow.com/questions/2268749/defining-global-constant-in-c
// Análisis de Pro vs Contras contra #define y otras formas

using namespace std;

class Block {
	private:
		// Atributos Seccion Header
		std::string pre_block;
		std::string txns_hash;	// <- retiene el hash256(hash256(cadena_prehash))
		unsigned int bits;	/* La dificultad de bits */
		unsigned int nonce;
		StatusBlock eBlock;
		// Atributos Seccion Body;
		unsigned int txn_count;
		lista <Transaction *> ListaTran;
		Transaction * CurTran;
		std::string cadena_prehash;
		// Métodos privados
		std::string RecalculoHash( void );

	public:
    // Métodos
        // Constructores
        Block();
		Block( const raw_t & raw );
		//Block( const & std::string previo_block, size_t bits, const & raw_t );
		// size_t bits sale de BlockChainManager::getUserDefinedDifficulty(void), pero referenciar a esta clase implica un encastramiento indeseado.
        // Destructor
        ~Block();
		// Getters
        unsigned int gettxn_count();
		std::string getpre_block();
		std::string gettxns_hash();
		unsigned int getbits();
		unsigned int getnonce();
		std::string getcadenaprehash();
		// Setters
		bool setpre_block( std::string valor );
		bool settxns_hash( std::string valor );		// Debo dejar el método de asignación. El cálculo Hash es externo al objeto block, no está encapsulado.
		bool setbits( unsigned int valor );
		bool setnonce( int valor );			// Debo dejar el método de asignación. El cálculo del Nonce es externo al objeto block, no está encapsulado.
		bool settransaction( const raw_t & raw ) ;  // TODO
		StatusBlock EstatusBlock();
};

#endif /* BLOCK_H_ */
