
#include "Common.h"
#include "Transaction.h"


	using namespace std;

	Transaction::Transaction()
		{
		// default is to use "standard" continuation char US (0x1F)
		UseAltContChar = false;
		}

	Transaction::~Transaction()
		{
		list<Block*>::iterator i = blockList.begin();

		for(; i != blockList.end(); ++i)
			{
			delete *i;
			}
		}


	
	void Transaction::SetAlternateBlockContinuationChar(bool option)
		{
		UseAltContChar = option;
		}


	void Transaction::AddField(const string& dat)
		{
		fields.push_back(dat);
		}


	void Transaction::GetNextField()
		{
		if(blockList.end() == blockIterator) return;

		char* blkPtr = (*blockIterator)->GetBlockBuffer();
		//first or continuation block
		}


	void Transaction::Blockify()
		{
		if(fields.empty()) 
			{
			return;
			}

		list<string>::iterator pos = fields.begin();
		Block* block = new Block();
		blockList.push_back(block);

		do
			{
			int bytesWritten = 0;
			int messageLength = pos->length();

			do
				{
				if(block->BytesFree() == 0)
					{
					//endblock, get new block
					block = block->EndBlock(false, UseAltContChar);
					blockList.push_back(block);
					}

				bytesWritten += block->AddField(*pos, bytesWritten);
				}
			while(messageLength != bytesWritten);

			//end field
			block->EndField();
			pos++;
			}
		while(pos != fields.end());

		//end last block;
		block = block->EndBlock(true, UseAltContChar);

		if(block) 
			{
			blockList.push_back(block);
			}
		}



	void Transaction::ResetBlockList()
		{
		blockIterator = blockList.begin();
		}


	Block* Transaction::GetNextBlock()
		{
		if(blockList.end() != blockIterator)
			{
			return *blockIterator++;
			}
		else
			{
			return NULL;
			}
		}



	Block::Block()
		{
		index = 0;
		block[index++] = kSTX;	//stx
		blockState = kOpen;
		lastBlock = false;
		}


	int Block::BytesFree() const 
		{
		int bytesFree;
		
		bytesFree = kBlockSize - (index + 5);

		if(bytesFree < 0) bytesFree = 0;

		return bytesFree;
		}
#pragma warning( push )
#pragma warning(disable:4996)

	int Block::WriteBlock(const std::string& dat, int offset, int length)
	{
		string wstr(dat.substr(offset, length));

		wstr.copy(&block[index], length);
		index += length;

		return length;
	}

#pragma warning( pop ) 


	unsigned int Block::ComputeChecksum()
		{
		unsigned int checksum = 0;
		unsigned char* ptr;
		ptr = reinterpret_cast<unsigned char*>(&block[0]);

		for(; ptr < reinterpret_cast<unsigned char*>(&block[index]); ++ptr)
			{
			checksum += (static_cast<unsigned int>(*ptr) & 0x7F);
			}
		return checksum;
		}



	void Block::WriteChecksum()
		{
		unsigned int checksum = ComputeChecksum();
		block[index++] = 0x30 + ((checksum >> 8) & 0xF);
		block[index++] = 0x30 + ((checksum >> 4) & 0xF);
		block[index++] = 0x30 + (checksum & 0xF);
		block[index++] = kCR;
		block[index++] = 0;
		}


	//add as many characters as possible to the current block.
	//starting at offset in in dat.

	int Block::AddField(const std::string& dat, int offset)
		{
		int bytesNeeded = dat.length() - offset;
		int bytesRemaining = BytesFree() - bytesNeeded;
		int bytesWritten;

		//string(offset) fits with characters to spare
		if(bytesRemaining > 1)
			{
			bytesWritten = WriteBlock(dat, offset, bytesNeeded);
			}
		//string(offset) fits exactly
		else if(bytesRemaining == 1)
			{
			bytesWritten = WriteBlock(dat, offset, bytesNeeded);
			blockState = kFieldEndExact;
			}
		//string(offset) will not fit in this block
		else
			{
			bytesWritten = WriteBlock(dat, offset, BytesFree());
			//set state used by end block method
			if(bytesRemaining == 0)
				blockState = kOverflow;
			else
				blockState = kFieldContinue;
			}
		return bytesWritten;
		}


	//writes an end of field character when appropriate
	void Block::EndField()
		{
		if((blockState == kOpen) || (blockState == kFieldEndExact))
			block[index++] = kCR; //end of field
		}


	// write the end of block character according to state, 
	//  and continuation character option.
	//
	Block* Block::EndBlock(bool lastBlock, bool useAltContKhar)
		{
		Block* retBlock;

		//create a new block
		if((!lastBlock) || (blockState == kOverflow))
			{
			retBlock = new Block();
			}
		else
			{
			//no block need, last one
			retBlock = NULL;
			lastBlock = true;
			}

		switch(blockState)
			{
			case kOverflow:
				//special case
				retBlock->WriteTerminator(lastBlock);
				//fall thru

			case kFieldContinue:
				//field does not fit in current block
				//continue in next

				if(true == useAltContKhar)
					{
					// alternate continuation char.
					block[index++] = kETB;   // ETB is correct for Z16
					}
				else
					{
					// standard continuation char.
					block[index++] = kUS;	// US is correct for TAP
					}

				// block[index++] = kUS;	// US is correct for TAP
				// block[index++] = kETB;   // ETB is correct for Z16

				WriteChecksum();
				break;

			case kOpen:
				//no more fields in transacion, end
				block[index++] = kETX;
				WriteChecksum();
				break;

			case kFieldEndExact:
				//field fills remainder of block
				if(lastBlock)
					{
					//last field in transaction
					block[index++] = kETX;
					WriteChecksum();
					}
				else
					{
					// additional fields continue in remaining blocks
					block[index++] = kETB;
					WriteChecksum();
					}
				break;
			}
		return retBlock;
		}



	//used in overflow state.
	//a field will fit in the current block, but the end of field
	//marker <CR> will not.  Place the <CR> in the new block.
	void Block::WriteTerminator(bool lastBlock)
		{
		block[index++] = kCR;
		if(lastBlock)
			{
			block[index++] = kETX;
			WriteChecksum();
			lastBlock = true;
			}
		}
