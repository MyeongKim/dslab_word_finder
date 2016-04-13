#include <stdio.h>
#include <string.h>	

#define true 1
#define false 0

void match_string(char* regex, char* word, int* result)
{
	int i=0;
	int j=0;
	while (*(regex+i) != '\0'){
		if (*(regex+i) == '*'){
			if(*(regex+i+1) != '\0'){
				// If * is not end expession
				i = i+1;
			}else{
				// If * is end expession
				*result = true;
				return;
			}
			while( *(regex+i) != *(word+j)){
				if(*(word+j) == '\0'){
					*result = false;
					return;
				}
				j = j+1;
			}
			match_string(regex+i, word+j, result);
			return;
		}else if(*(regex+i) == '?'){
			if(*(regex+i+1) != '\0'){
				// If regex is not ended
				i = i+1;
				if((*(regex+i) != *(word+j)) && (*(regex+i) != *(word+j+1))){
					*result = false;
					return;
				}

				if (*(regex+i) == *(word+j+1)){
					j = j+1;
				}
				j = j+1;
			}else{
				// If ? is end expression
				if(*(word+j) == '\0' || *(word+j+1) == '\0'){
					*result = true;
					return;
				}else{
					*result = false;
					return;
				}
			}
		}else{
			if(*(word+j) != *(regex+i)){
				*result = false;
				return;
			}else{
				match_string(regex+i+1, word+j+1, result);
				return;
			}
		}
		i = i+1;
	}

	if(*(word+j) == '\0'){
		*result = true;
		return;
	}else{
		*result = false;
		return;
	}
}

int main(void)
{
	FILE* ptr_file;
	char buf[1000];
	char input[1000];
	char seps[] = " \t\n,.\"\'\?";
	char* token;
	int line_num=1;
	int line_start=0;
	int match_result=0;
	char token_array[1000][1000];
	int match_count=0;
	int match_count_sum=0;
	int i;

	ptr_file = fopen("sample.txt", "r");
	if (!ptr_file)
		return 1;

	printf("단어를 입력하세요 : ");
	scanf("%s", input);
	
	// Get one line and save to buffer
	 while(fgets(buf, 1000, ptr_file) != NULL){
		token = strtok(buf, seps);
		match_count = 0;
		while(token != NULL){
			match_result = 0;
			match_string(input, token, &match_result);
			if(match_result == 1){
	 			if(line_start == 0){
					printf("\nline#%d: ", line_num);
					line_start = 1;
				}
				strcpy(token_array[match_count], token);
				match_count = match_count + 1;
			}
			token = strtok(NULL, seps);
		}
		
		if(match_count != 0){
			// print matched words from token_array
			for(i=0; i< match_count; i++){
				printf("%s", token_array[i]);
				if ( i != match_count-1){
					printf(", ");
				}
			}
		}

		match_count_sum = match_count_sum + match_count;

		
		line_start = 0;
		line_num = line_num + 1;
	}
	printf("\n\n총 %d 개의 %s 를 찾았습니다. \n", match_count_sum, input);
	printf("\n");
	fclose(ptr_file);
	return 0;
}
