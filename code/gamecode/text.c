int JStrLen(char *txt)	//Check

{
  int ret;
  int count;
  char p;
  
  count = 0;
  p = *txt;
  while ((p != '\0' && (txt[1] != '\0'))) {
    if ((txt[2] == 'B') &&
       (((txt[3] == 'D' && (ret = CombinationCharacterBD(p,txt[1]), ret != 0)) ||
        ((txt[3] == 'C' && (ret = CombinationCharacterBC(*txt,txt[1]), ret != 0)))))) {
      txt = txt + 4;
    }
    else {
      txt = txt + 2;
    }
    p = *txt;
    count = count + 1;
  }
  return count;
}

void AddSpacesIntoText(char *txt,uint bits)

{
  int j;
  int i;
  int cnt;
  
  if (Game.language == 'c') {
    j = 0;
    cnt = 0;
    if (*txt != '\0') {
      i = j;
      do {
        buf.159[i] = txt[cnt];
        j = i + 1;
        if ((((bits & 1) != 0) || ((((bits & 2) != 0 && ('/' < txt[cnt])) && (txt[cnt] < ':')))) | |
           (((bits & 4) != 0 && (txt[cnt] == '-')))) {
          buf.159[i + 1] = ' ';
          j = i + 2;
        }
        cnt = cnt + 1;
        i = j;
      } while (txt[cnt] != '\0');
    }
    buf.159[j] = '\0';
    strcpy(txt,buf.159);
  }
  return;
}

char * GetStringIdx(int param_1,int param_2)

{

		//TODO

}


