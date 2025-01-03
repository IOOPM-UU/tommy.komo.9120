

int ask_question_int(char *question)
{

  int result = 0;
  int conversions = 0;
  do
    {
      printf("%s\n", question);
      conversions = scanf("%d", &result);
      int c;
      do
        {
          c = getchar();
        }
      while (c != '\n' && c != EOF);
      putchar('\n');
    }
  while (conversions < 1);
  return result;
}


int read_string(char *buf, int buf_siz){
  
  int counter = 0;
  char c;

  while(counter < (buf_siz-1)){
    c = getchar();
    
    if(c == '\n' || c == EOF ) {
      break;
    }
    buf[counter] = c;
    counter++;
  }
  buf[counter] = '\0';

  return counter; 
}