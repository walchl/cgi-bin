#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <string.h>


char *utility_Fetch_Query_By_GET(){
	return getenv("QUERY_STRING") ;
}


char *utility_Fetch_Query_By_POST(){
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


char *utility_Duplicate(const char *src){
	char *dst = (char*) malloc( (strlen(src)+1)*sizeof(char) ) ;
	strcpy( dst, src ) ;
	return dst ;
}


// If POST is used, the Unicode character has the format "&#xxxxx;"
// So feel free to use utility_trim() to process Unicode
char *utility_Trim(char *str, const char *split){
	// scan str[]
	char *seek ;
	for( seek=str ; *seek ; seek++ ){
		const char *sp ;
		for( sp=split ; *sp ; sp++ ){
			if( *sp == *seek )
				break ;
		}
		if( *sp )
			break ;
	}

	// if any char in split[] is found in str[]
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	// return next starting location
	return seek ;
}


// parse query string: (key)=(value)(split)(key)=(value)...(split)(key)=(value)
char *utility_Parse_Query( char *str, char **key, char **value, const char *split ){
	char *next = utility_Trim( str, split ) ;

	*key = str ;
	*value = utility_Trim( *key, "=" ) ;

	return next ;
}


// generate a unsigned 32-bit token from char user[] and char pass[]
unsigned int utility_Gen_Token( const char *user, const char *pass ){
	unsigned int token = 0 ;

	int i ;
	for( i=0 ; user[i] ; i++ )
		token ^= ((unsigned int)user[i])<< ((i*9)%29) ;
	for( i=0 ; pass[i] ; i++ )
		token ^= ((unsigned int)pass[i])<< ((i*9)%29) ;

	return token ;
}

#endif
