#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_db.h"
#include "utility.h"

#define FORM	"form.xml"

char _firstname[] = "First" ;
char _lastname[] = "Last" ;
char *firstname = _firstname ;
char *lastname = _lastname ;
char token[17] ;


// Process Query
void process( char *query ){
	// char array for record insertion
	char record[MAX_RECORD_LEN] ;
	record[0] = '\0' ;
	char *record_end = &record[0] ;

	// flag for database clearing
	int clear_db = 0 ;

	// parse query
	char *seek = query ;
	while( *seek ){
		// get each (key, value) pair
		char *key, *value ;
		seek = parse_query( seek, &key, &value ) ;

		// if key is "lastpage"
		if( !strcmp(key, "lastpage") ){
			if( !strcmp(value, "clear-db") ){
				clear_db = 1 ;
				break ;
			}
		}
		// if key is "token"
		else if( !strcmp(key, "token") ){
		}
		// other keys
		else{
			// append (key, value) to record
			sprintf( record_end, "\t%s:\"%s\"", key, value ) ;
			for( ; *record_end ; record_end++ ) ;

			// modify names
			if( !strcmp(key, "firstname") )
				firstname = value ;
			else if( !strcmp(key, "lastname") )
				lastname = value ;
		}
	}

	// if database should be removed
	if( clear_db )
		db_clear() ;
	else if( record[0] )
		db_append( record ) ;
}


void print_form( int count ){
	char xml[1024] = {0} ;

	FILE *fin = fopen( FORM, "r" ) ;
	if( fin )
		fscanf( fin, "%1024c", xml ) ;
	fclose( fin ) ;

	if( xml[0] ){
		gen_token( token, sizeof(token), firstname, lastname ) ;
		printf( xml, firstname, lastname, count, token ) ;
	}
}


void web_out(){
	printf("<html>\n<body>\n") ;

	// process query
	char *query = getenv("QUERY_STRING") ;
	printf( "%s=\"%s\"<br><br>\n", "QUERY_STRING", query ) ;
	if( query && *query )
		process( query ) ;

	// show database
	int count = db_show() ;

	// print forms
	print_form( count ) ;

	printf("</body>\n</html>\n") ;
}


int main(){
	web_out() ;

	return 0 ;
}
