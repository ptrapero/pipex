/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 20:40:36 by ptrapero          #+#    #+#             */
/*   Updated: 2025/04/14 23:43:24 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*conc;

	i = 0;
	j = 0;
	conc = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (conc == NULL)
		return (NULL);
	while (s1[j] != '\0')
	{
		conc[j] = s1[j];
		j++;
	}
	while (s2[i] != '\0')
	{
		conc[j] = s2[i];
		i++;
		j++;
	}
	conc[j] = '\0';
	return (conc);
}

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*temp;
	char	*conc;

	temp = ft_strjoin(s1, s2);
	conc = ft_strjoin(temp, s3);
	free (temp);
	return (conc);
}
