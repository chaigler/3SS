//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

/*
	-Mat these are the magic numbers that tell the compiler how big each node is
*/

#include "console/compiler.h"

	#define NODE_SIZE_BREAKSTMTNODE_LOOPCOUNT				2
	#define NODE_SIZE_BREAKSTMTNODE							0

	#define NODE_SIZE_CONTINUESTMTNODE_LOOPCOUNT			2
	#define NODE_SIZE_CONTINUESTMTNODE						0

	#define NODE_SIZE_RETURNSTMTNODE						1

	#define NODE_SIZE_IFSTMTNODE							2

	#define NODE_SIZE_LOOPSTMTNODE							2

	#define NODE_SIZE_CONDITIONALEXPRNODE					4

	#define NODE_SIZE_STREQEXPRNODE							2

	#define NODE_SIZE_STRCATEXPRNODE						2

	#define NODE_SIZE_COMMACATEXPRNODE						2

	#define NODE_SIZE_INTUNARYEXPRNODE						1
	#define NODE_SIZE_INTUNARYEXPRNODE_REQUINT				2

	#define NODE_SIZE_FLOATUNARYEXPRNODE_REQUINT			2
	#define NODE_SIZE_FLOATUNARYEXPRNODE					1

	#define NODE_SIZE_VARNODE								3
	#define NODE_SIZE_VARNODE_REQSTRING						6

	#define NODE_SIZE_INTNODE								2
	#define NODE_SIZE_INTNODE_REQNONE						0

	#define NODE_SIZE_FLOATNODE								2
	#define NODE_SIZE_FLOATNODE_REQNONE						0

	#define NODE_SIZE_STRCONSTNODE							2
	#define NODE_SIZE_STRCONSTNODE_DOC						2
	#define NODE_SIZE_STRCONSTNODE_REQSTRING				2
	#define NODE_SIZE_STRCONSTNODE_REQNONE					0

	#define NODE_SIZE_CONSTANTNODE							2
	#define NODE_SIZE_CONSTANTNODE_REQSTRING				2
	#define NODE_SIZE_CONSTANTNODE_REQNONE					0

	#define NODE_SIZE_ASSIGNEXPRNODE						3
	#define NODE_SIZE_ASSIGNEXPRNODE_ARRARYINDEX			6
	#define NODE_SIZE_ASSIGNEXPRNODE_ARRARYINDEX_REQSTRING	8

	#define NODE_SIZE_ASSIGNOPEXPRNODE						8
	#define NODE_SIZE_ASSIGNOPEXPRNODE_ARRAYINDEX			5

	#define NODE_SIZE_FUNCCALLEXPRNODE						5

	#define NODE_SIZE_SLOTACCESSNODE						1
	#define NODE_SIZE_SLOTACCESSNODE_REQSTRING				3

	#define	NODE_SIZE_SLOTACCESSNODE_REQSTRING_ARRAYEXPR	3

	#define NODE_SIZE_INTERNALSLOTACCESSNODE				3

	#define NODE_SIZE_SLOTASSIGNNODE						1
	#define NODE_SIZE_SLOTASSIGNNODE_REQSTRING				5
	#define NODE_SIZE_SLOTASSIGNNODE_ARRAYEXPR				3

	#define NODE_SIZE_SLOTASSIGNOPNODE						6
	#define NODE_SIZE_SLOTASSIGNOPNODE_ARRAYEXPR			9

	#define NODE_SIZE_OOBJECTDECLNODE_SUBOBJECT_NAME		2
	#define NODE_SIZE_OOBJECTDECLNODE_SUBOBJECT				10

	#define NODE_SIZE_OBJECTDECLNODE						2

	#define NODE_SIZE_FUNCTIONDECLSTMTNODE					8
	#define NODE_SIZE_FUNCTIONDECLSTMTNODE_PER_ARG			0