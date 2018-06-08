#ifndef __UTILITY_H__
#define __UTILITY_H__


char *query_by_get(){
	return getenv("QUERY_STRING") ;
}


char *query_by_post(){
	static int capacity = 0 ;
	static char *buffer = NULL ;

	// get length of query
	int query_len = atoi( getenv("CONTENT_LENGTH") ) ;
	if( query_len == 0 )
		return NULL ;

	// enlarge capacity if necessary
	if( capacity < query_len ){
		if( buffer )
			free( buffer ) ;

		capacity = query_len *2 ;
		buffer = (char*) malloc( capacity ) ;
	}

	// reset buffer
	memset( buffer, 0, query_len ) ;

	// read query
	char scanf_format[8] ;
	sprintf( scanf_format, "%%%dc", query_len ) ;
	scanf( scanf_format, buffer ) ;

	return buffer ;
}


char *parse_query( char *seek, char **key, char **value, char split ){
	// set *key
	*key = seek ;

	//find the end of key
	for( ; *seek && *seek!='=' ; seek++ ){
		// Unicode detection "&#xxxxx;"
		if( *seek=='&' && *(seek+1)=='#' ){
			for( ; *seek && *seek!=';' ; seek++ ) ;
			if( !*seek )
				break ;
		}
	}
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	// set *value
	*value = seek ;

	// find the end of value
	for( ; *seek && *seek!=split ; seek++ ){
		// Unicode detection "&#xxxxx;"
		if( *seek=='&' && *(seek+1)=='#' ){
			for( ; *seek && *seek!=';' ; seek++ ) ;
			if( !*seek )
				break ;
		}
	}
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	return seek ;
}


unsigned int gen_token( const char *user, const char *pass ){
	unsigned int token = 0 ;

	int i ;
	for( i=0 ; user[i] ; i++ )
		token ^= ((unsigned int)user[i])<< ((i*9)%29) ;
	for( i=0 ; pass[i] ; i++ )
		token ^= ((unsigned int)pass[i])<< ((i*9)%29) ;

	return token ;
}

#endif
