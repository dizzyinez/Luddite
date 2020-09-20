PShape loadOBJ(int i)
{
  String load_string = "character_"; 
  if ( i < 10)
  {
    load_string += "00000" + i;
  }
  else if ( i < 100)
  {
    load_string += "0000" + i;
  }
  else if ( i < 1000)
  {
    load_string += "000" + i;
  }
  else if ( i < 10000)
  {
    load_string += "00" + i;
  }
  else if ( i < 100000)
  {
    load_string += "0" + i;
  }
  else
  {
    load_string += i;
  }
  load_string += ".obj";
  return loadShape(load_string);
}
