/*
 * sepa_producer.h
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
 * @brief Header file defining C producer functions for SEPA
 * @file sepa_producer.h
 * @author Francesco Antoniazzi <francesco.antoniazzi@unibo.it>
 * @date 12 Apr 2017
 *
 * This file contains the functions useful to build-up a SEPA producer. We provide function for updates.
 * Requires libcurl. You should not include it in your code only if you are writing a plain consumer or aggregator. In this case, it
 * is strongly suggested to import the related header file.
 * @see sepa_consumer.h
 * @see sepa_aggregator.h
 * @see https://curl.haxx.se/libcurl/
 * @see http://zserge.com/jsmn.html
 * @todo Support for https
 */

#ifndef SEPA_PRODUCER
#define SEPA_PRODUCER

#include "sepa_utilities.h"
#include "sepa_secure.h"

#define KPI_PRODUCE_FAIL		-1
#define HTTP					0
#define HTTPS					1

/**
 * @brief Send update to SEPA
 * 
 * @param update_string: the sparql containing the update to be sent to SEPA
 * @param http_server: the address of the SEPA engine, for example "http://sepa.org:1234/sparql"
 * @return a long containing the http response code
 */
long kpProduce(const char * update_string,const char * http_server,sClient * jwt);

#endif // SEPA_PRODUCER
