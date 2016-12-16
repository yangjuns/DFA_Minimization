meet: hash.c hash.h meet.c
	@gcc meet.c hash.c hash.h -o meet

clean:
	@rm meet
