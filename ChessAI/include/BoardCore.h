#pragma once
#include "Core.h"
#include "BitBoard.h"
#include <array>

enum Piece
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NUM_PIECES
};

enum Colour
{
	WHITE = 0,
	BLACK = 8,
	NUM_COLOURS = 2
};

size_t PosToIndex(size_t x, size_t y);

BitBoard MaskPawnAttacks(Colour colour, size_t square);
BitBoard MaskKnightAttacks(size_t square);
BitBoard MaskKingAttacks(size_t square);
BitBoard MaskBishopAttacks(size_t square);
BitBoard MaskRookAttacks(size_t square);

BitBoard GenerateBishopAttacks(size_t square, BitBoard blockers);
BitBoard GenerateRookAttacks(size_t square, BitBoard blockers);

BitBoard SetOccupancy(size_t index, size_t relevantBits, BitBoard attackMask);

BitBoard FindMagicNumber(size_t square, size_t relevantBits, Piece bishopOrRook);




constexpr static size_t NUM_BITBOARDS = 16;

// precomputed boards for performancec
constexpr BitBoard notAFile = BitBoard(18374403900871474942ULL);
constexpr BitBoard notHFile = BitBoard(9187201950435737471ULL);

constexpr BitBoard notABFile = BitBoard(18229723555195321596ULL);
constexpr BitBoard notHGFile = BitBoard(4557430888798830399ULL);

constexpr std::array<size_t, BOARD_DIM * BOARD_DIM> bishopBitCount = 
{
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 
	6, 5, 5, 5, 5, 5, 5, 6
};

constexpr std::array<size_t, BOARD_DIM * BOARD_DIM> rookBitCount =
{
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};

// precomputed magics with _seed = 1804289383, BOARD_DIM = 8
#ifdef PREMAGICS
constexpr std::array<uint64_t, BOARD_DIM * BOARD_DIM> bishopMagics =
{
	1134702728396804ULL,
	12992902663319323724ULL,
	1299314883997745160ULL,
	38316124818903092ULL,
	2312783135198285824ULL,
	2306412874081763328ULL,
	1153089248985907200ULL,
	212240120741888ULL,
	18058388716132352ULL,
	10403317355517313032ULL,
	9008303346614337ULL,
	2703444562201813024ULL,
	2312599594067820561ULL,
	2256284329967978ULL,
	9297717178567098368ULL,
	720612297288056841ULL,
	1168793280512ULL,
	34377351190ULL,
	71441632320ULL,
	1152921530377831104ULL,
	1442560396494856ULL,
	3530828739305869312ULL,
	38285080812650656ULL,
	4503603993706626ULL,
	2487183297856274612ULL,
	6917777534583177224ULL,
	72062130630951936ULL,
	1809325548494716928ULL,
	286962286592ULL,
	9295429907951677952ULL,
	1152922674989903872ULL,
	27022422400182272ULL,
	4794008136613889ULL,
	144203166190667776ULL,
	9368615435529945088ULL,
	1125973191950338ULL,
	4792225828790468608ULL,
	17663057215496ULL,
	180218803476533764ULL,
	4616526069150386320ULL,
	18018135198138640ULL,
	576602143042052096ULL,
	653587108402299904ULL,
	9802225343657427584ULL,
	2323857631078252554ULL,
	8802871021576ULL,
	81630088868659202ULL,
	158939572871680ULL,
	397455938698873090ULL,
	4516802415820803ULL,
	9223380835106816032ULL,
	141845606993920ULL,
	19210667697938824ULL,
	1126046540170880ULL,
	2305845277241835649ULL,
	4662006884352128ULL,
	4627449169171189784ULL,
	51791396855874850ULL,
	2666135377752379904ULL,
	13988180442746996992ULL,
	4611729998901185088ULL,
	18049584492478752ULL,
	144124053156839680ULL,
	1133890831874ULL
};

constexpr std::array<uint64_t, BOARD_DIM * BOARD_DIM> rookMagics =
{
	144132919852597248ULL,
	18032128138674176ULL,
	288375598810923010ULL,
	1188954768392323200ULL,
	288232850591056390ULL,
	288407466277076992ULL,
	288247973715134464ULL,
	2341872030720262656ULL,
	845533040082944ULL,
	1126329976160256ULL,
	667676254157668352ULL,
	9296573123300204544ULL,
	5188287525399592960ULL,
	140883517505536ULL,
	140741917689856ULL,
	18049585331061760ULL,
	1026965851212939264ULL,
	72093603581657090ULL,
	137707651092ULL,
	4543182382071862ULL,
	36451559307673600ULL,
	9245890722454962176ULL,
	4778320442689586176ULL,
	4612002712140157440ULL,
	72058736633708544ULL,
	2224809869315ULL,
	2305843299660890113ULL,
	9223375336195497985ULL,
	171933447236ULL,
	76121458826287104ULL,
	4935945243171356672ULL,
	20066356723712ULL,
	432363723353554945ULL,
	6917533631848120322ULL,
	12106947984931962882ULL,
	1374524276736ULL,
	4611725600913113089ULL,
	2306988700465106944ULL,
	1152934984445603840ULL,
	13835058063888928768ULL,
	2017683689556803590ULL,
	9223372312269717521ULL,
	551379140611ULL,
	9232379803049656321ULL,
	44594102272ULL,
	1447907288823496720ULL,
	2595199302468960256ULL,
	4539788951552ULL,
	13835058330168459392ULL,
	72057731481075974ULL,
	10376295759984202240ULL,
	144396731772576000ULL,
	288230394036292096ULL,
	85568410256179712ULL,
	576460756883604488ULL,
	579838488818614336ULL,
	180148951151943782ULL,
	432345839383478532ULL,
	3494794412500733964ULL,
	585467969358794753ULL,
	585467963371425924ULL,
	9232418819636003852ULL,
	1179943136748634628ULL,
	38298330757366786ULL
};
#else
static std::array<uint64_t, BOARD_DIM * BOARD_DIM> bishopMagics;
static std::array<uint64_t, BOARD_DIM * BOARD_DIM> rookMagics;
static void InitMagics()
{
	for (int square = 0; square < BOARD_DIM * BOARD_DIM; ++square)
	{
		bishopMagics[square] = FindMagicNumber(square, bishopBitCount[square], BISHOP).to_ullong();
	}

	for (int square = 0; square < BOARD_DIM * BOARD_DIM; ++square)
	{
		rookMagics[square] = FindMagicNumber(square, rookBitCount[square], ROOK).to_ullong();
	}
}
#endif
