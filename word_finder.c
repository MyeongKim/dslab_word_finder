#include <stdio.h>
#include <string.h>	

void match_string(char*, char*, int*);

void reg_question(char* regex, char* word, int* result)
{
	// 검색어글자가 현재 글자와 같지 않으면 다음 글자와 비교
	// 현재 글자가 마지막 글자면 그대로 유지
	if(*regex != *word && *word != '\0')
		word = word + 1;
	match_string(regex, word, result);
	return;
}

void match_string(char* regex, char* word, int* result)
{
	if(*regex == '*')
	// 검색어글자가 '*' 인 경우
	{
		do{
			// 다음 검색어와 현재 글자를 비교
			// '*' 검색어는 '?' 검색어를 여러번 반복한 것과 같다고 해석된다.
			reg_question(regex+1, word, result);
			// 전의 검색에서 끝이 나지 않았을 경우 루프를 반복
			if(*result != 1 && *word != '\0')
				word = word + 1;
		}
		while(*(regex+1) != *word && *word != '\0'&& *result != 1);
		return;
	}else if(*regex == '?')
	// 검색어 글자가 '?' 인 경우
	{
		reg_question(regex+1, word, result);
		return;
	}else if(*regex == '\0' && *word == '\0')
	// 검색어 글자가 끝났고 현재 글자도 마지막 글자인 경우
	// 성공
	{
		*result = 1;
		return;
	}else if(*word != *regex)
	// 검색어 글자가 '*'/'?' 가 아니면서 현재 글자와 다른 경우
	// 실패
	{
		*result = 0;
		return;
	}else{
	// 검색어 글자가 현재 글자와 일치하는 경우
	// 다음 단어를 검색한다.
		match_string(regex+1, word+1, result);
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
	int i,j;

	for(i=0; i<5;i++)
	// 검색을 5회 반복한다.
	{
		ptr_file = fopen("sample.txt", "r");
		if (!ptr_file)
			return 1;
		
		// 검색어를 입력받는다.
		printf("단어를 입력하세요 : ");
		scanf("%s", input);

		// 일치하는 전체 단어 갯수를 0으로 초기화
		match_count_sum=0;	
		// sample.txt 파일을 한 줄씩 읽는다.
		while(fgets(buf, 1000, ptr_file) != NULL)
		{
			token = strtok(buf, seps);
			// 이번 줄에서 일치하는 단어 갯수를 0으로 초기화
			match_count = 0;
			while(token != NULL)
			{
				// 결과를 나타내는 변수를 -1로 초기화
				// 1이면 성공, 0이면 실패
				match_result = -1;

				// 검색어와 토큰을 매개변수로 기능 실행
				match_string(input, token, &match_result);
				if(match_result == 1)
				{
		 			if(line_start == 0)
					{
						printf("\nline#%d: ", line_num);
						line_start = 1;
					}
					// 매치되는 단어를 배열에 넣는다.
					strcpy(token_array[match_count], token);
					match_count = match_count + 1;
				}
				token = strtok(NULL, seps);
			}
			
			if(match_count != 0)
			// 이번 라인에서 매치된 단어가 있을경우
			{
				// 배열에 담겨있는 단어들을 출력한다.
				for(j=0; j< match_count; j++)
				{
					printf("%s", token_array[j]);
					if ( j != match_count-1)
					{
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
	}	
	return 0;
}
