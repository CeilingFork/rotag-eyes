#include<stdio.h>
#include<string.h>

/*
Transforms a trigram(given as an array of 3 chars) into a unified form
ie transform(123)=transform(312)=transform(231)=123

How it works:
-first it finds the smallest value glyph in the trigram(min)
-then an occurrence of this minimal glyph that is preceeded by a different glyph
	(this is essentially to "group" all of the minimal glyphs at the start of the representation eg.: transform(121)->112
-now "rotate" the trigram such that the glyph found in the previous step is at the start(index 0) and the order of the glyphs is preseved

example for a few values:

321:
-minimal glyph is '1'
-starting_point is 2
- rotation: 321 -> 132

132:
-minimal glyph is '1'
-starting point is 0
-rotation: 132 -> 132
(Notice how this is a fixed point of the function, this means that we can find all of the valid symbols by looping through all possible trigrams
 and selecting the ones whose transformed is themself)
 
232
-minimal glyph is '2'
-starting point is 2(since the '2' at 0 is preceeded by a '2')
-rotation: 232 -> 223

322
-minimal glyph is '2'
-starting point is 1
-rotation: 322 -> 223
*/
void transform(char trigram[3])
{
	char min = trigram[0];
	for(int i = 1; i < 3; ++i)//find minimal glyph value
	{
		if(trigram[i]<min)min=trigram[i];
	}
	int starting_point = 0;
	for(int i = 0; i < 3; ++i) //Find a glyph whose value is minimal and who is preceded by a glyph not equal to it(ie greater since it has the minimal value)
	{
		if(trigram[i]==min && trigram[(i-1)%3]!=min)starting_point=i;
	}
	//The rotation
	char temp[3] = {trigram[starting_point],trigram[(starting_point+1)%3], trigram[(starting_point+2)%3]};
	memcpy(trigram,temp,3);
	
	return;
}

void build_magic_index_table(char(*table)[4])
{
	size_t index = 0;
	char temp[4] = {0};
	char temp2[4]= {0};
	for(int i = 0; i < 5*5*5;++i)
	{
		temp[0] = (i/(5*5))%5+'0';
		temp[1] = (i/5)%5+'0';
		temp[2] = i%5+'0';
		strcpy(temp2,temp);
		transform(temp2);
		if(strcmp(temp,temp2)==0) //if you remove this conditional it'll instead list all trigrams and their transformed values
		{
			strcpy(table[index++],temp2);
		}
	}
	return;
}

int frequency_analysis_or_something_like_that(FILE*source,FILE*output_file)
{
	
	int frequencies[5][5][5]={0};
	int c = fgetc(source);
	char message_name[32] = {0};
	int index = 0;	
	while(c!=',')//Read the message name
	{
		message_name[index++]=c;
		c=fgetc(source);
		if(c==EOF)return 1;
	}
	char buffer[3];
	index=0;
	while(c!='\n')//Scan through the glyphs
	{
		c=fgetc(source);
		if(c==EOF){printf("EOF?!\n");break;}
		buffer[index++]=c;
		if(index==3)//After an entire trigram has been read
		{
			index=0;
			transform(buffer);//Transform the read trigram into the proper form(See transform for more information)
			frequencies[buffer[0]-'0'][buffer[1]-'0'][buffer[2]-'0']+=1;//Add the occurrence to the frequency table
		}
	}
	fprintf(output_file,"%s\n",message_name);
	int magic_index = 0;
	for(int i = 0; i < 5;++i){		//Loop through all trigrams in the table and print their frequency
		for(int j = 0; j < 5;++j){
			for(int k = 0; k < 5;++k)
			{
				char trigram[4] = {i+'0',j+'0',k+'0',0};
				char copy[4];
				strcpy(copy,trigram);
				transform(copy);
				if(strcmp(copy,trigram)==0)//Only print fixed points of transform
					fprintf(output_file,"%1d%1d%1d\t%d\t%d\n",i,j,k,magic_index++,frequencies[i][j][k]);
	}}}
	return 0;
}

typedef enum RepresentationFormat {
	TRIGRAM_FORMAT=0, //Uses the trigram produced by transform 
	ASCII_INDEX=1,	//Uses the "magic index" of the above described trigram in ASCII+32
	DECIMAL_INDEX=2	//Directly uses the "magic index"
	} RepresentationFormat;
/*                           
This function is for translating the cyphertext(contained in the file pointed to by source) to a rotationally agnostic representation.
The exact representation used is determined by the parameter format(see the enum defined above)
*/
void translate(FILE*source, RepresentationFormat format,FILE*output_file)
{
	char magic_index_table[45][4]={0};
	build_magic_index_table(magic_index_table);
	
	char buffer[4]={0};
	int index = -1;
	for(char c = fgetc(source); c!=EOF; c=fgetc(source))
	{
		//Just print the name part normally
		if(index==-1)
		{
			fputc(c,output_file);
			if(c==',')index=0;
		}
		else
		{
			if(c=='\n')
			{
				fputc('\n',output_file);
				index=-1;
				continue;
			}
			buffer[index++]=c;
			if(index==3)
			{
				transform(buffer);
				switch(format)
				{
					default:
					case TRIGRAM_FORMAT:
						fprintf(output_file," %3s",buffer);
						break;
					case ASCII_INDEX:
						int aindex=0;
						while(aindex<45 && strcmp(buffer,magic_index_table[aindex])!=0)++aindex;
						fprintf(output_file,"%c",aindex+32);
						break;
					case DECIMAL_INDEX:
						int dindex=0;
						while(dindex<45 && strcmp(buffer,magic_index_table[dindex])!=0)++dindex;
						fprintf(output_file," %02d",dindex);
						break;
				}
				index=0;
			}
		}
	}
	return;
}

void list(FILE*output_file)
{
	//This the trigrams used for the rotationally agnostic representation
	char temp[4] = {0};
	char temp2[4]= {0};
	for(int i = 0; i < 5*5*5;++i)
	{
		temp[0] = (i/(5*5))%5+'0';
		temp[1] = (i/5)%5+'0';
		temp[2] = i%5+'0';
		strcpy(temp2,temp);
		transform(temp2);
		if(strcmp(temp,temp2)==0)
		{
			fprintf(output_file,"%s\n",temp2);
		}
	}
	return;
}

int main(int argc, char*argv[])
{
	FILE*input_file=NULL;
	FILE*output_file=stdout;
	for(int i = 1; i < argc; ++i)
	{
		if(strncmp(argv[i],"o:",2)==0)
		{
			output_file=fopen(argv[i]+2,"w");
			if(output_file==NULL)
			{
				printf("Failed to open output file \"%s\", falling back to stdout\n",argv[i]+2);
				output_file=stdout;
			}
		}
		if(strncmp(argv[i],"i:",2)==0)
		{
			input_file=fopen(argv[i]+2,"r");
			if(input_file==NULL)
			{
				printf("Failed to open input file \"%s\", falling back to raw.txt\n",argv[i]+2);
				input_file=NULL;
			}
		}
	}
	if(input_file==NULL)
	{
		FILE*input_file=fopen("raw.txt","r");
		if(input_file==NULL)
		{
			puts("Failed to open file:\"raw.txt\"");
			return 1;
		}
	}
	printf(	"0 exit"
			"\n1 list frequencies\n"
			"2 list each uique symbol in the rotationally agnostic representation"
			"\n3 translate cyphertext to the rotationally agnostic representation\n");
	int answer;
	while(scanf("%d",&answer)!=1);
	switch(answer)
	{
		case 0:
			break;
		case 1:
			while(frequency_analysis_or_something_like_that(input_file,output_file)==0);
			break;
		case 2:
			list(output_file);
			break;
		case 3:
			RepresentationFormat f;
			while(printf("Which format?(0=trigram, 1=ascii+32, 2=decimal index):"),scanf("%d",&f)!=1 && (f<0 || f>2));
			translate(input_file,f,output_file);
			break;
		case 4:
			char magic_index_table[45][4]={0};
			build_magic_index_table(magic_index_table);
			for(int i = 0; i < 45;++i)
				fprintf(output_file,"%s\n",magic_index_table[i]);
			
			
	}
	fclose(input_file);
	fclose(output_file);
	return 0;
}