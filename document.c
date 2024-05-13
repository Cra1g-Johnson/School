#include <stdio.h>
#include <string.h>
#include "document.h"


/* Finished */
int init_document(Document *doc, const char *name){    
    if( doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE){
        return FAILURE;
    }
    
    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);
    return SUCCESS;
}

/* Finished */
int reset_document(Document *doc){
    if(doc == NULL){
        return FAILURE;
    }
    /* this limits printing and does not delete data */    
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}


/* Finished */
int print_document(Document *doc){
    int i, j; 
    if(doc == NULL){
        return FAILURE;
    }

    printf( "Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
    for(i = 0 ; i < doc->number_of_paragraphs ; i++){
        for( j = 0 ; j < doc->paragraphs[i].number_of_lines ; j++){
            printf("%s\n", doc->paragraphs[i].lines[j]);
        }
        /* this ensures proper formating when a para has no lines or it is last para */
        if(j > 0 && i < doc->number_of_paragraphs - 1){
            printf("\n");
        }
    }
    return SUCCESS;
}

/* Finished*/
int add_paragraph_after(Document *doc, int paragraph_number){
    Paragraph *p, *r;
    if( doc == NULL || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    if(doc->number_of_paragraphs == MAX_PARAGRAPHS){
        return FAILURE;
    }
    if(paragraph_number < doc->number_of_paragraphs){
        p = doc->paragraphs + paragraph_number;
        r = p + (doc->number_of_paragraphs - paragraph_number);
        while(r != p){
            *r = *(r - 1);
            r--;
        }
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    } else {
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    }
    
    doc->number_of_paragraphs += 1;
    return SUCCESS;
}

/* param check finished , must change how handles cases */
int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line){
   if(doc == NULL || new_line == NULL || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    if(doc->paragraphs[paragraph_number].number_of_lines == 20 || line_number > 20){
        return FAILURE;
    }
    /* this function should handle two cases
        - lines added in order 
        - lines added out of order 
       right now it only handles the first
    */
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line); 
    doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
    return SUCCESS;
}

/* Finished */
int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines){
    if( doc == NULL || number_of_lines == NULL || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    *number_of_lines = doc->paragraphs[paragraph_number-1].number_of_lines;
    return SUCCESS;
}

/* Finished */
int append_line(Document *doc, int paragraph_number, const char *new_line){
    int line_number;
    if(doc == NULL || new_line == NULL || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    if(doc->paragraphs[paragraph_number].number_of_lines == 20){
        return FAILURE;
    }

    line_number = doc->paragraphs[paragraph_number - 1].number_of_lines;
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
    return SUCCESS;
}

/* Finished */
int remove_line(Document *doc, int paragraph_number, int line_number){
    int i, num_lines = doc->paragraphs[paragraph_number-1].number_of_lines;
    if(doc == NULL || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    if(line_number > num_lines){
        return FAILURE;
    }
    if(line_number == num_lines){
        doc->paragraphs[paragraph_number - 1].lines[line_number -1][0] = '\0';
    } else {
        for(i = line_number ; i < num_lines ; i++){
            strcpy(doc->paragraphs[paragraph_number - 1].lines[i-1], 
            doc->paragraphs[paragraph_number - 1].lines[i]);
        }
    }
    doc->paragraphs[paragraph_number - 1].number_of_lines -= 1;
    return SUCCESS;
}

/* this seems to work put there is definitly a better way, revisit */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
    int i, paragraph = 0, line = 0;
    if(doc == NULL || data == NULL || data_lines == 0){
        return FAILURE;
    }
    /* this adds lines into respecting paragraphs*/
    for(i = 0 ; i < data_lines ; i++){
        if(data[i][0] == '\0'){
            paragraph++;
            line = 0;
        } else {
            strcpy(doc->paragraphs[paragraph].lines[line], data[i] );
            line++;
        }
        doc->paragraphs[paragraph].number_of_lines = line;
    }
    doc->number_of_paragraphs = paragraph + 1;
    return SUCCESS;
}

/* functional could fall into an infinite loop if replacement contains target */
int replace_text(Document *doc, const char *target, const char *replacement){
    int i, j, k, t_length, r_length; 
    char *p, *r;
    if( doc == NULL || target == NULL || replacement == NULL){
        return FAILURE;
    }
    t_length = strlen(target);
    r_length = strlen(replacement);
    for(i = 0 ; i < doc->number_of_paragraphs ; i++){
        for( j = 0 ; j < doc->paragraphs[i].number_of_lines ; j++){
            if( strstr(doc->paragraphs[i].lines[j], target) != NULL){
                p = strstr(doc->paragraphs[i].lines[j], target);
                if((t_length - r_length) > 0){
                    for(k = 0 ; k < r_length ; k++){
                        *(p + k) = replacement[k];
                    }
                    p = p + r_length;
                    r = p + 1;
                    while(*r != '\0'){
                        *p = *r;
                        p++;
                        r++;
                    }
                    *p = '\0';
                } else if ((t_length - r_length) < 0){
                    r = doc->paragraphs[i].lines[j];
                    while(*r != '\0'){
                        r++;
                    }
                    while(r != p){
                        *(r + (r_length - t_length)) = *r;
                        r--;
                    }
                    for(k = 0 ; k < r_length ; k++){
                        *(p + k) = replacement[k];
                    }
                } else {
                    for(k = 0 ; k < r_length ; k++){
                        *(p + k) = replacement[k];
                    }
                }
                j--; /* read line again to check for multiple instances*/
            }
        }
    }
    return SUCCESS;
}

/* Finished */
int highlight_text(Document *doc, const char *target){
    int i, j, k, t_length, add_length; 
    char *p, *r;
    if( doc == NULL || target == NULL ){
        return FAILURE;
    }
    t_length = strlen(target);
    add_length = strlen(HIGHLIGHT_START_STR);
    for(i = 0 ; i < doc->number_of_paragraphs ; i++){
        for( j = 0 ; j < doc->paragraphs[i].number_of_lines ; j++){
            p = doc->paragraphs[i].lines[j];
            while(strstr(p, target) != NULL){
                p = strstr(p, target);
                
                r = doc->paragraphs[i].lines[j];
                while(*r != '\0'){
                    r++;
                }
                while(r != p){
                    *(r + (add_length * 2)) = *r;
                    r--;
                }
                
                for(k = 0 ; k < add_length ; k++){
                    *p = HIGHLIGHT_START_STR[k];
                    p++;
                }
                for(k = 0 ; k < t_length ; k++){
                    *p = target[k];
                    p++;
                }
                for(k = 0 ; k < add_length ; k++){
                    *p = HIGHLIGHT_END_STR[k];
                    p++;
                }
            }
        }
    }
    return SUCCESS;
}

/* Finished */
int remove_text(Document *doc, const char *target){
    int i, j, length;
    char *p, *r; 
    if( doc == NULL || target == NULL){
        return FAILURE;
    }
    length = strlen(target);
    for(i = 0 ; i < doc->number_of_paragraphs ; i++){
        for( j = 0 ; j < doc->paragraphs[i].number_of_lines ; j++){
            if(strstr(doc->paragraphs[i].lines[j], target) != NULL){
                p = strstr(doc->paragraphs[i].lines[j], target);
                r = p + length;
                while( *r != '\0'){
                    *p = *r;
                    p++;
                    r++;
                }
                *p = '\0';
                j--; /* read line again to check for multiple instances */
            }
        }
    }
    return SUCCESS;
}

