定义两个vector,一个储存玩家，一个储存狼人，判定某一个玩家为狼人后就将这个玩家添加到狼人的vector当中
对于每一个玩家，我们用一个字段status表示这名玩家的身份，如果status=-1,那么这名玩家是狼人，如果status等于0，那么这名玩家的身份未确定，如果status等于1，那么这名玩家身份为人类
还定义一个字段honesty,如果玩家说的是真话，那么honesty=1,如果是假话，那么honesty=-1
回溯过程，首先判断第一名玩家说的是真话还是假话，

先对每一个人的身份做一个假设，假设第一个人为狼人，看第一个人的status状态，如果为0，那么更新身份，再假设这个人说的是真话会不会导致矛盾，如果不会，进入回溯，如果会，那就假设这个人说假话会不会导致冒顿，如果不会，进入回溯，如果会，这个人的身份假设不成立，换一种身份假设
如果为1，假设为-1，假设与当前身份不符，就换一种假设，然后进入回溯