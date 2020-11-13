/*
 * BlockChainFileManager.cpp
 *
 *  Created on: 25 oct. 2020
 *      Author: Ramiro
 */

#include "BlockChainFileManager.h"


BlockChainFileManager::BlockChainFileManager() {
	pRawData = NULL;
}

BlockChainFileManager::~BlockChainFileManager() {
	if(this->pRawData != NULL){
		pRawData->inTx = 0;
		delete [] pRawData->IN_tableOfTxId; 		pRawData->IN_tableOfTxId    = NULL;
		delete [] pRawData->IN_tableOfIndex;		pRawData->IN_tableOfIndex   = NULL;
		delete [] pRawData->IN_tableOfAddr;			pRawData->IN_tableOfAddr    = NULL;
		pRawData->outTx = 0;
		delete [] pRawData->OUT_tableOfValues;		pRawData->OUT_tableOfValues = NULL;
		delete [] pRawData->OUT_tableOfAddr;		pRawData->OUT_tableOfAddr   = NULL;
		delete pRawData;
		pRawData = NULL;
	}
}
status_t BlockChainFileManager::validate(std::istream * iss){
	if( ! this->isEmpty(iss)){
			int inTxTotal,outTxTotal;
		if( this->isTxIndexFromStream(iss,'\n',&inTxTotal) == false ) 	return STATUS_CORRUPT_FORMAT_BAD_TXIN;
		for(int inTx = 0 ; inTx < inTxTotal ; inTx++){
			if( this->isHashFromStream(iss,' ') == false ) 				return STATUS_CORRUPT_FORMAT_BAD_HASH;
			if( this->isTxIndexFromStream(iss,' ') == false ) 			return STATUS_CORRUPT_FORMAT_BAD_TXINDEX;
			if( this->isHashFromStream(iss) == false ) 					return STATUS_CORRUPT_FORMAT_BAD_HASH;
		}
		if( this->isTxIndexFromStream(iss,'\n',&outTxTotal) == false ) 	return STATUS_CORRUPT_FORMAT_BAD_TXOUT;
		for(int outTx = 0 ; outTx < outTxTotal ; outTx++){
			if( this->isBTCValueFromStream(iss,' ') == false ) 			return STATUS_CORRUPT_FORMAT_BAD_BTCVALUE;
			if( this->isHashFromStream(iss) == false ) 				    return STATUS_CORRUPT_FORMAT_BAD_HASH;
		}
		if( this->isEofFromStream(iss) == false ) 						return STATUS_CORRUPT_FORMAT;
	}
	return STATUS_OK;
}

bool BlockChainFileManager::isEmpty(std::istream  * iss)
{
	// PRECONDICION: ESTA FUNCION SOLO DEBE USARSE ANTES DE HACER
	// EL TRABAJO DEL ARCHIVO PUESTO QUE AL TERMINAR DEJA APUNTANDO
	// AL PRINCIPIO

	bool empty;
	//Voy al final de File
	iss->seekg(0, ios::end);
	empty = (iss->tellg() == 0)? true: false;
	//Vuelvo al principio del File
	iss->clear();
	iss->seekg(0, iss->beg);
	return empty;

}

bool BlockChainFileManager::isTxIndexFromStream(std::istream *iss,char delim , int * pValue)
{
	int IndexValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	if ((ss >> IndexValue).fail())	  return false;
	if (IndexValue < 0) 			  return false;
	//Debug
	//std::cout << line << std::endl;
	if(pValue != NULL) *pValue = IndexValue;
	return true;
}

bool BlockChainFileManager::isHashFromStream(std::istream *iss,char delim, std::string * pString)
{
	std::string line;
	std::stringstream ss;
	std::getline(*iss, line,delim);
	if( line.back() == '\r'){  				//Para portabilidad Linux - Window
		line.substr(0, line.size()-1);
	}
	if ( line.size() != (size_t) LargoHash::LargoHashEstandar ) 	return false;
	for (unsigned int i = 0; i < line.length(); ++i) {
		if ( ! isxdigit ( line[i] ) ) 								return false;
	}
	//Debug
	//std::cout << line << std::endl;
	if(pString != NULL) *pString = line;
	return true;

}

bool BlockChainFileManager::isBTCValueFromStream(std::istream *iss,char delim,float * pFloat)
{
	float floatValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	if ((ss >> floatValue).fail())	  return false;
	if (floatValue < 0) 			  return false;
	//Debug
	//std::cout << line << std::endl;
	if(pFloat != NULL) *pFloat = floatValue;
	return true;
}

bool BlockChainFileManager::isEofFromStream(std::istream *iss){
	std::string line;
	if (iss->eof()) return true;
	try{
		if (std::getline(*iss, line,'\r').fail() ) return true;
	}catch(const std::ios_base::failure& ex) {
		//std::cerr << "Caught: std::ios_base::failure" << std::endl;
		iss->clear();
		return true;
	}
	if (line.size() != 0 )
		return false;
	return true;
}


status_t BlockChainFileManager::parse(std::istream * iss, raw_t * &pBuilderRawData){

	//Creo el archivo raw_t en el entorno del filemanager
	this->pRawData = new raw_t{0};
	if ( ! this->isEmpty(iss)){
		if(pRawData == NULL) return STATUS_BAD_ALLOC;
		pRawData->inTx = this->getTxIndexFromStream(iss,'\n');

		pRawData->IN_tableOfTxId = new std::string[pRawData->inTx];
		pRawData->IN_tableOfIndex = new int[pRawData->inTx];
		pRawData->IN_tableOfAddr = new std::string[pRawData->inTx];
		if(		pRawData->IN_tableOfTxId == NULL  ||
				pRawData->IN_tableOfIndex == NULL ||
				pRawData->IN_tableOfAddr == NULL ) 	return STATUS_BAD_ALLOC;

		for(int i = 0; i < pRawData->inTx; i++)
		{
			pRawData->IN_tableOfTxId[i]  = this->getHashFromStream(iss,' ');
			pRawData->IN_tableOfIndex[i] = this->getTxIndexFromStream(iss,' ');
			pRawData->IN_tableOfAddr[i]  = this->getHashFromStream(iss);
		}

		pRawData->outTx = this->getTxIndexFromStream(iss,'\n');
		pRawData->OUT_tableOfValues = new float[pRawData->outTx];
		pRawData->OUT_tableOfAddr = new std::string[pRawData->outTx];
		if(		pRawData->OUT_tableOfValues == NULL  ||
				pRawData->OUT_tableOfAddr   == NULL  ) 	return STATUS_BAD_ALLOC;

		for(int i = 0; i < pRawData->outTx; i++)
		{
			pRawData->OUT_tableOfValues[i]  = this->getBTCValueFromStream(iss,' ');
			pRawData->OUT_tableOfAddr[i] = this->getHashFromStream(iss);
		}
	}
	pBuilderRawData = this->pRawData;

	return STATUS_OK;
}


int BlockChainFileManager::getTxIndexFromStream(std::istream *iss,char delim)
{
	int IndexValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	ss >> IndexValue;
	return IndexValue;
}

std::string BlockChainFileManager::getHashFromStream(std::istream *iss,char delim)
{
	std::string line;
	std::stringstream ss;
	std::getline(*iss, line,delim);
	return line;
}

float BlockChainFileManager::getBTCValueFromStream(std::istream *iss,char delim)
{
	float floatValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	ss >> floatValue;
	return floatValue;
}



status_t BlockChainFileManager::convert(std::ostream * iss, const lista <Block *> & BlockChain){
	lista <Block *> ::iterador it(BlockChain);
	std::string obtainedHash;

	if(!iss->good())						return STATUS_BAD_READ_OUTPUT_FILE;
	if( BlockChain.vacia() )				return STATUS_NO_BLOCKS_TO_CONVERT;
	while(!it.extremo()){
		*iss << it.dato()->getpre_block() << '\n';
		*iss << it.dato()->gettxns_hash() << '\n';
		*iss << it.dato()->getbits( )	  << '\n';
		*iss << it.dato()->getnonce()	  << '\n';
		*iss << it.dato()->gettxn_count() << '\n';
		*iss << it.dato()->getcadenaprehash();	
		it.avanzar();
	}
	return STATUS_FINISH_CONVERT_SUCCESSFULY;
}


