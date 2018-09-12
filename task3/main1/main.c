#include<stdio.h>
#include "change.h"

int main() {
	char a;
	printf("请输入小写字母：");
	a = getchar();
	while ('\n' != getchar()) {
		continue;
	}
	while (a<'a' || a>'z'){
		printf("输入的字母不符合，请重新输入:");
		a = getchar();
		while ('\n' != getchar()) {
			continue;
		}
	}
	printf("转换后的大写字母为:%c\n", change(a));
	return 0;

}
