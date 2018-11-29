#if !defined(_TRANACTION_H_)
#define _TRANACTION_H_
#pragma once


#pragma warning(disable:4786)
#pragma warning(disable:4284)

#include <memory>
#include <list>
#include <string>

class Block;

class Transaction
	{
	public:
		Transaction();
		~Transaction();

		void SetAlternateBlockContinuationChar(bool);

		void AddField(const std::string&);
		void GetNextField();

		void Blockify();

		void ResetBlockList();
		Block* GetNextBlock();

	private:
		std::list<std::string> fields;
		std::list<Block*> blockList;
		std::list<Block*>::iterator blockIterator;

		bool UseAltContChar;
	};



class Block
	{
	public:
		Block();

		int BytesFree()const;
		int AddField(const std::string& dat, int offset);

		void EndField();
		Block* EndBlock(bool lastBlock, bool useAltContChar);

		char* GetBlockBuffer(){return block;}
		bool IsLastBlock()const {return lastBlock;}

		enum Ctrl{kSTX=2, kETX=3, kCR=0x0D, kETB=0x17, kUS=0x1F};
		enum BlockState{kOpen, kFieldContinue, kFieldEndExact, kOverflow};
		enum BufferMax{kBlockSize = 256};
		// enum BufferMax{kBlockSize = 50};

	private:
		int WriteBlock(const std::string& dat, int offset, int length);
		void WriteChecksum();
		unsigned int ComputeChecksum();
		void WriteTerminator(bool lastBlock);


	private:
		BlockState blockState;
		bool lastBlock;
		int index;
		char block[kBlockSize + 1];
	};
#endif