#ifndef __SIMPLE_DB_H__
#define __SIMPLE_DB_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"


#define DB_MSG		"db_msg.txt"
#define DB_NAME		"db_name.txt"

#define MAX_VALUE_LEN	1024

#define KEYS	3
char db_record_key[KEYS][16] = {
		"firstname",
		"lastname",
		"msg" } ;

int db_record_num = 0 ;
char ***db_record_value = NULL ;
/*
db_record_value[i][j][]: the i-th record has its j-th value to the j-th key.

You may consider using struct:
typedef struct{
	char *firstname ;
	char *lastname ;
	char *msg ;
} Record ;
Record *db_record ;

Then use:
db_record[i].firstname
db_record[i].lastname
db_record[i].msg
*/


void db_Write_All(){
	// if db is not loaded, abort.
	if( !db_record_value )
		return ;

	// open files
	FILE *fout_msg = fopen( DB_MSG, "w" ) ;
	FILE *fout_name = fopen( DB_NAME, "w" ) ;

	// write all records
	if( fout_msg && fout_name ){
		fprintf( fout_msg, "%d\n", db_record_num ) ;

		int i ;
		for( i=0 ; i<db_record_num ; i++ ){
			if( db_record_value[i] ){
				// write msg
				fprintf( fout_msg, "%s\n", db_record_value[i][2] ) ;
				// write firstname, lastname
				fprintf( fout_name, "%s\t%s\n", db_record_value[i][0], db_record_value[i][1] ) ;
			}
		}

		fclose( fout_msg ) ;
		fclose( fout_name ) ;
	}

	// free memory
	if( db_record_value ){
		int i, j ;
		for( i=0 ; i<db_record_num ; i++ ){
			for( j=0 ; j<KEYS ; j++ )
				free( db_record_value[i][j] ) ;
			free( db_record_value[i] ) ;
		}
		free( db_record_value ) ;
		db_record_value = NULL ;
	}
	db_record_num = 0 ;
}


void db_Read_All(){
	// if db is loaded, skip.
	if( db_record_value )
		return ;

	char line[MAX_VALUE_LEN] ;

	// open files
	FILE *fin_msg = fopen( DB_MSG, "r" ) ;
	FILE *fin_name = fopen( DB_NAME, "r" ) ;

	// set db_record_num
	if( fin_msg && fin_name ){
		fgets( line, sizeof(line), fin_msg ) ;
		sscanf( line, "%d", &db_record_num ) ;
	}

	// allocate db_record_value
	db_record_value = (char***) malloc( (db_record_num+1) * sizeof(char**) ) ;

	// read all records
	if( fin_msg && fin_name ){
		int i, j ;
		for( i=0 ; i<db_record_num ; i++ ){
			// prepare record
			char **record = (char**) malloc( KEYS*sizeof(char*) ) ;
			for( j=0 ; j<KEYS ; j++ )
				record[j] = NULL ;
			db_record_value[i] = record ;

			// read fin_msg
			if( fgets( line, sizeof(line), fin_msg ) ){
				utility_Trim( line, "\n" ) ;
				record[2] = utility_Duplicate( line ) ;
			}

			// read fin_name
			if( fgets( line, sizeof(line), fin_name ) ){
				char *firstname = line ;

				// split firstname
				char *lastname = utility_Trim( firstname, "\t\n") ;
				record[0] = utility_Duplicate( firstname ) ;

				// split lastname
				utility_Trim( lastname, "\t\n" ) ;
				record[1] = utility_Duplicate( lastname ) ;
			}

			// final check record
			for( j=0 ; j<KEYS ; j++ ){
				if( record[j] == NULL )
					record[j] = utility_Duplicate( "" ) ;
			}
		}

		fclose( fin_msg ) ;
		fclose( fin_name ) ;
	}
}


void db_Print_All(){
	db_Read_All() ;

	int i, j ;
	for( i=0 ; i < db_record_num ; i++ ){
		printf( "#%d:", i ) ;
		for( j=0 ; j<KEYS ; j++ )
			printf( " %s:\"%s\"", db_record_key[j], db_record_value[i][j] ) ;
		printf( "<br>\n" ) ;
	}
}


void db_Append( const char **record_value ){
	db_Read_All() ;

	char **record = (char**) malloc( KEYS*sizeof(char*) ) ;
	db_record_value[db_record_num] = record ;
	db_record_num ++ ;

	int j ;
	for( j=0 ; j<KEYS ; j++ ){
		if( record_value[j] )
			record[j] = utility_Duplicate( record_value[j] ) ;
		else
			record[j] = utility_Duplicate( "" ) ;
	}

	db_Write_All() ;
}


void db_Clear(){
	remove( DB_MSG ) ;
	remove( DB_NAME ) ;
}


void db_Del(int id){
	db_Read_All() ;

	if( id<0 || db_record_num<=id )
		return ;

	int i, j ;

	// free memory
	for( j=0 ; j<KEYS ; j++ )
			free( db_record_value[id][j] ) ;
	free( db_record_value[id] ) ;

	// shift
	for( i=id+1 ; i<db_record_num ; i++ )
		db_record_value[i-1] = db_record_value[i] ;
	db_record_num -- ;

	db_Write_All() ;
}

#endif
