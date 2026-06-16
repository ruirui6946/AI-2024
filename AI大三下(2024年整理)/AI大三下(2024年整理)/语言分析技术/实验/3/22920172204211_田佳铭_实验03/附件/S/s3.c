
int min(int i, int j) 
{
  if (i < j) return i; else return j;
}
  
  
void main(void)
{
  int i;
  int j;
  j=6;
  i=j+4;
  i=4+6;
  j=min(i, "wrong");
}