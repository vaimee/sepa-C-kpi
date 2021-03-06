/*
 * sepa_utilities.h
 * 
 * Copyright 2017 Francesco Antoniazzi <francesco.antoniazzi@unibo.it>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 
 */

/**
 * @brief Header file defining json-parsing-related routines useful for communication with SEPA
 * @file sepa_utilities.h
 * @author Francesco Antoniazzi <francesco.antoniazzi@unibo.it>
 * @date 12 Apr 2017
 *
 * This file contains functions necessary in the various parts of the sepa apis in C. It requires jsmn, and
 * performs json parsing, json interpretation and information retrieval.
 * @see http://zserge.com/jsmn.html
 * @see https://curl.haxx.se/libcurl/
 */

#ifndef SEPA_UTILITIES
#define SEPA_UTILITIES
#define JSMN_STRICT

#include "jsmn.h"

#define COMPLETE_JSON					1
#define INCOMPLETE_JSON					0
#define PARSING_ERROR					-100
#define PING_JSON						-101
#define SUBSCRIPTION_ID_JSON			-102
#define NOTIFICATION_JSON				-103
#define UNSUBSCRIBE_CONFIRM				-104
#define QUERY_START_BUFFER				512
#define IDENTIFIER_ARRAY_LEN			58
#define IDENTIFIER_LAST_INDEX			57
#define IDENTIFIER_STRING_LEN			"%56s"
#define SUBSCRIPTION_ID_PREAMBLE_LEN	13
#define _initSepaNode()					{.bindingName=NULL,.type=UNKNOWN,.value=NULL}

#define BINDING_LEN			6
#define BINDING_NAME		2
#define BINDING_TYPE		5
#define BINDING_VALUE		7

#define URI_STRING			"uri"
#define LITERAL_STRING		"literal"
#define BNODE_STRING		"bnode"

#define HTTP_200_OK					200
#define HTTP_201_CREATED				201
/**
 * @brief sepaNode type: URI, LITERAL, BNODE, or unknown upon initialization or error
 */
typedef enum field_type {
	URI,
	LITERAL,
	BNODE,
	UNKNOWN
} FieldType;

/**
 * @brief a sepaNode is association of the binding name, the type and the current value.
 */
typedef struct SEPA_Node {
	char *bindingName; /**< The variable association */
	FieldType type; /**< The type of the node */
	char *value; /**< The current value of the node */
} sepaNode;

/**
 * @brief Useful structure for tracking notifications
 */
typedef struct notification_properties {
	int sequence; /**< Sequence number of the notification */
	char identifier[IDENTIFIER_ARRAY_LEN]; /**< The subscription identifier */
	// head:vars?
} notifyProperty;

/**
 * @brief Structure useful for receiving json data with libcurl.
 */
typedef struct http_json_result {
	size_t size; /**< Json number of characters */
	char *json; /**< Json received */
} HttpJsonResult;

/**
 * @brief inits curl libraries.
 * You may want to call this function if you are using the secure mode, or if you are
 * doing queries. This function just inhibits useless multiple calls 'curl_global_init'
 * @see http_client_free
 * @see curl_global_init documentation
 */ 
int http_client_init();

/**
 * @brief frees curl-allocated memory.
 * You may want to call this function if you are using the secure mode, or if you are
 * doing queries. This function just inhibits useless multiple calls 'curl_global_init' and 'curl_global_cleanup'
 * @see http_client_free
 * @see curl_global_init documentation
 */ 
void http_client_free();

/**
 * @brief Transforms to string the json token.
 * 
 * From a jsmn parsed json we get the needed token.
 * @param json: it's the json itself
 * @param token: the json item we need
 * @param destination: pointer to string in which the token will be written
 * @return EXIT_SUCCESS or PARSING_ERROR
 */
int getJsonItem(char * json,jsmntok_t token,char ** destination);

/**
 * @brief fprintf for sepaNodes
 * 
 * Prints to stream the content of a sepaNode array
 * @param outstream: FILE* to which write
 * @param nodeArray: array of sepaNode
 * @param arraylen: length of nodeArray
 * @param prefix: a string to be printed before outputs
 */
void fprintfSepaNodes(FILE * outstream,sepaNode * nodeArray,int arraylen,const char * prefix);

/**
 * @brief Free memory utility
 * @param nodeArray: array of sepaNode to be freed
 * @param arraylen: length of nodeArray
 */
void freeSepaNodes(sepaNode * nodeArray,int arraylen);

/**
 * @brief Constructor of a sepaNode
 * @param node_bindingName: string containing the name of the variable binding
 * @param node_type: string between (URI_STRING)"uri",(LITERAL_STRING)"literal" or (BNODE_STRING)"bnode"
 * @param node_value: current value of the node
 * @return the sepaNode formed
 * @see FieldType
 */
sepaNode buildSepaNode(char * node_bindingName,char * node_type,char * node_value);

/**
 * @brief parse a subscription result json
 * 
 * From a subscription result json, we obtain an array of sepaNode for added results, one for removed results, the respective length and
 * accessory information of the notification.
 * @param jsonResults: a well-formed subscription result json
 * @param addedNodes: pointer to an array of sepaNode, in which to store added bindings
 * @param addedlen: pointer to int, in which to store length of addedNodes
 * @param removedNodes: pointer to an array of sepaNode, in which to store removed bindings
 * @param removedlen: pointer to int, in which to store length of removedNodes
 * @param data: pointer to notifyProperty in which to store sequence number and subscription id
 * @return PARSING_ERROR if any error occurs or JSMN_ERROR_PART, JSMN_ERROR_INVAL (see jsmn documentation); PING_JSON if the packet is a ping from SEPA;
 * NOTIFICATION_JSON if it is a standard subscription notification; UNSUBSCRIBE_CONFIRM if the packet is an unsubscription confirm;
 * SUBSCRIPTION_ID_JSON if the packet is a subscription confirmation; 
 * @see http://zserge.com/jsmn.html error codes
 */
int subscriptionResultsParser(char * jsonResults,sepaNode ** addedNodes,int * addedlen,sepaNode ** removedNodes,int * removedlen,notifyProperty * data);

/**
 * @brief parse a query result json
 * 
 * From a query result json, we obtain an array of sepaNode.
 * @param jsonResults: a well formed query result json
 * @param results: a pointer to an array of sepaNode, in which to store bindings
 * @param resultlen: a pointer to int, in which to store results length
 * @return PARSING_ERROR, or JSMN_ERROR_PART, JSMN_ERROR_INVAL (see jsmn documentation) upon failure; otherwise EXIT_SUCCESS;
 * @see http://zserge.com/jsmn.html error codes
 */
int queryResultsParser(char * jsonResults,sepaNode ** results,int * resultlen);

/**
 * @brief internal routine for getting bindings from a json, both query or subscription.
 * 
 * The json is parsed looking for sepa-compliant sequences.
 * @param json: a well formed sepa-compliant json
 * @param tokens: a jsmn array of tokens obtained parsing "json" with jsmn
 * @param outlen: the length of the output sepaNode array
 * @return a sepaNode array containing all the bindings
 */
sepaNode * getResultBindings(char * json,jsmntok_t * tokens,int * outlen);

/**
 * @brief internal useful routine that accumulates in parameter 'data' the output coming from an http call made with libcurl.
 * @see https://curl.haxx.se/libcurl/
 * @see HttpJsonResult
 */
size_t queryResultAccumulator(void * ptr, size_t size, size_t nmemb, HttpJsonResult * data);

#endif 
