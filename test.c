#include <stdio.h>
#include <setjmp.h>

#define gotfoo goto

int globa;

void f(int a, int b)
{
   int abcdef = 123;
   int azerty;

   abcdef = 42;

   azerty = 456;

   printf("%d", a + b);

   {
      int c = 2;
      int c2;
      static int stata;
      extern int globa;

      if(a + b + c == 0)
      {
	 gotfoo foo456;
      }
   }

   for(; abcdef < 5; abcdef++)
   {
       if(abcdef == 2)
       {
           continue;
       }

       if(abcdef == 3)
       {
           break;
       }

       if(abcdef == 4)
       {
           return;
       }

       puts("bar");
   }

   puts("blah");

   f(a - 1, b);

  foo456:
   puts("foo");
}

typedef void (*hptr)(void);

void h(void)
{
    hptr foo = h;

    foo();
    h();
    h();
}

#define g(a) _g(a)

int _g(int a)
{
    if(a == 1)
    {
        return 2;
    }
    else if(a == 2)
    {
        return 3;
    }

    g(3);

    return 1;
}

void i(int a)
{
    int b = (a == 4) ? 3 : 6;

    if(a == 1)
    {
        puts("foo");

        if(a == 4)
        {
            puts("foo2");
        }
    }
    else
    {
        puts("foo3");
    }
}

typedef char charfoo;

void j()
{
    charfoo foogetch = getchar();
    unsigned char ucgetch = getchar();
    int igetch = getchar();
    char cgetch = getchar();
    jmp_buf env;
    int a = 0;
    int abcsetj = setjmp(env);

    if(a = setjmp(env))
    {
        puts("foo");
    }

    if(setjmp(env))
    {
        puts("blah");
    }

    if((a = setjmp(env)))
    {
        puts("foo");
    }
}

int main(void)
{
   f(1, 2);
   g(1);
   h();
   i(1);
   j();

   return 0;
}
