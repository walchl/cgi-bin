#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

typedef enum {NONE, BY_ID, BY_NAME, BY_SCORE} Cmp ;


typedef struct{
	int id ;
	const char *name ;
	int score ;
} Record ;

void set(Record *this, int id, const char *name, int score){
	this->id = id ;
	this->name = name ;
	this->score = score ;
}

int smaller_than( const Record *this, const Record *b, Cmp cmp ){
	if( cmp == BY_ID )
		return this->id < b->id ;
	if( cmp == BY_NAME )
		return this->name < b->name ;
	if( cmp == BY_SCORE )
		return this->score > b->score ;
	return 0 ;
}


Record record[5] ;
Record *order[5] ;


void sort_by( Cmp cmp ){
	int i, j ;
	for( i=0 ; i<5 ; i++ )
		order[i] = &record[i] ;

	for( i=0 ; i<5 ; i++ ){
		int end = 5-i ;
		for( j=1 ; j<end ; j++ ){
			if( smaller_than(order[j], order[j-1], cmp) ){
				Record *tmp = order[j] ;
				order[j] = order[j-1] ;
				order[j-1] = tmp ;
			}
		}
	}
}


void process( char *query ){
	// parse query
	char *seek = query ;
	while( seek && *seek ){
		// get each (key, value) pair
		char *key, *value ;
		seek = parse_query( seek, &key, &value, '&' ) ;

		// if key is "sort_by"
		if( !strcmp(key, "sort_by") ){
			if( !strcmp(value, "id") ){
				sort_by( BY_ID ) ;
				return ;
			}
			if( !strcmp(value, "name") ){
				sort_by( BY_NAME ) ;
				return ;
			}
			if( !strcmp(value, "score") ){
				sort_by( BY_SCORE ) ;
				return ;
			}
		}
	}

	sort_by( NONE ) ;
}


void init(){
	set( &record[0], 3, "Alice", 50 ) ;
	set( &record[3], 5, "Bob", 60 ) ;
	set( &record[4], 2, "Carol", 90 ) ;
	set( &record[2], 1, "Dan", 70 ) ;
	set( &record[1], 4, "Erin", 80 ) ;
}


void web_out(){
	printf("<html>\n<body>\n<table>\n") ;

	printf( "<tr>\n" ) ;
	printf( "<td><b><a href='/cgi-bin/sort_table.exe?sort_by=%s'>%s</a></b></td>\n", "id", "ID" ) ;
	printf( "<td><b><a href='/cgi-bin/sort_table.exe?sort_by=%s'>%s</a></b></td>\n", "name", "NAME" ) ;
	printf( "<td><b><a href='/cgi-bin/sort_table.exe?sort_by=%s'>%s</a></b></td>\n", "score", "SCORE" ) ;
	printf( "</tr>\n" ) ;

	// process query
	char *query = query_by_get() ;
	process( query ) ;

	int i ;
	for( i=0 ; i<5 ; i++ ){
		printf( "<tr>" ) ;
		printf( "<td>%d</td>", order[i]->id ) ;
		printf( "<td>%s</td>", order[i]->name ) ;
		printf( "<td>%d</td>", order[i]->score ) ;
		printf( "</tr>\n" ) ;
	}

	printf("</table>\n</body>\n</html>\n") ;
}


int main(){
	init() ;
	web_out() ;

	return 0 ;
}
