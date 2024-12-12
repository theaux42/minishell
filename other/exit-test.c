#include <stdio.h>

int	ft_atoi(const char *str)
{
	int	num;
	int	i;
	int	neg;

	num = 0;
	neg = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + str[i] - '0';
		i++;
	}
	return (num * neg);
}

int main(int argc, char *argv[])
{
  int return_value = 0;

  if (argc != 2)
  {
    printf("[EXIT_TEST] Exiting with status 0.\n");
    return 0;
  }
  
  return_value = ft_atoi(argv[1]);
  printf("[EXIT_TEST] Exiting with status %i.\n", return_value);
  return return_value;
}

