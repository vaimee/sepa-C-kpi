#ifndef SEPA_UTILITIES
#define SEPA_UTILITIES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMPLETE_JSON		1
#define INCOMPLETE_JSON		0

typedef enum field_type {
	URI,
	LITERAL,
	BNODE
} FieldType;

typedef struct SEPA_Node {
	char *bindingName;
	FieldType type;
	char *value;
} sepaNode;

void freeSepaNodes(sepaNode * nodeArray,int arraylen);
sepaNode buildSepaNode(const char * node_bindingName,FieldType node_type,const char * node_value);
int subscriptionResultsParser(const char * jsonResults,sepaNode * addedNodes,int * addedlen,sepaNode * removedNodes,int * removedlen);
int queryResultsParser(const char * jsonResults,sepaNode * results,int * resultlen);
char * kpQuery(const char * sparql_query,const char * sparql_address);
int checkReceivedJson(char * myjson);

#endif 
