#include<stdio.h>

int main() {
	char a;
	printf("请输入一个小写字母：");
	scanf("%c", &a);
	while ('\n' != getchar()) {
		continue;
	}
	while (a<'a' || a>'z'){
		printf("输入的字母不符合，请重新输入:");
		scanf("%c", &a);
        while ('\n' != getchar()) {
			continue;
		}
	}
	printf("转换后的大写字母为:%c\n", a - 32);
	return 0;

}
