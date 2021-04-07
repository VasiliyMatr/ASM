
### __This is the 6th lab in ded32's 2nd semester course__

### ___The tasks:___
* Realize chained hash table.
* Test different hash functions dispercion uniformity to choose the best one for next task.
* Optimize element search function.  

### ___Hash Table realization:___

My hash table unoptimized code is here:
https://github.com/VasiliyMatr/ASM/blob/master/T06HASH/unoptimized

I have used ded32's word set to test my hash table, you can find it here:
https://github.com/VasiliyMatr/ASM/blob/master/T06HASH/data/word_set.txt

Also, hash table size were specially choosed too tiny for test word set. It has helped to find bad optimized code later.

### ___Hash functions tests:___

I've tested six hash functions:
* One value hash func - returns 0 in all cases
* First letter hash func - returns first letter code
* Letters sum hash func - returns all letters codes sum
* Letters avg hash func - returns all letters codes avg value
* My hash func used in 3rd lab of ded32's 1st semester course
* Crc32 hash func

Here are the results:

![](data/oneValStat.png)     |  ![](data/firstLetterStat.png)
------------------------------|------------------------------

![](data/lettersAvgStat.png) | ![](data/lettersSumStat.png)
------------------------------|------------------------------

![](data/myStat.png)         | ![](data/crc32Stat.png)
------------------------------|------------------------------

I decided to use crc32 for next task, but i would like to note that my hash function showed pretty good results to.

### ___Optimization:___

I used callgrind utility to profile my program and time utility to compare execution times of optimized and unoptimized versions.

Here is the first time test results:

![](data/firstCmp.png)

Here is the first profile retults:

![](data/firstProfile.png)

As you can see, crc32 hash function has used ~96% of all execution time.

I've used _mm_crc32_u8 function to optimize crc32 hash function code.

Were:
```c++

HashTableKey_t crc32Hash( HashableData_t hashableData )
{
  /* Not initing data for speed */
    unsigned int crc32Table[256];
    unsigned int crc32Hash;
    int i, j;

    int len = strlen (hashableData);

    for (i = 0; i < 256; i++)
    {
        crc32Hash = i;
        for (j = 0; j < 8; j++)
            crc32Hash = crc32Hash & 1 ? (crc32Hash >> 1) ^ 0xEDB88320UL : crc32Hash >> 1;

        crc32Table[i] = crc32Hash;
    };

    crc32Hash = 0xFFFFFFFFUL;

    while (len--)
        crc32Hash = crc32Table[(crc32Hash ^ *hashableData++) & 0xFF] ^ (crc32Hash >> 8);

    return crc32Hash ^ 0xFFFFFFFFUL;
}

```

With _mm_crc32_u8 usage:

```c++

HashTableKey_t crc32Hash( HashableData_t hashableData )
{
    int            symbolId = 0;
    char           symbol   = hashableData[0];
    HashTableKey_t hash     = 0xFFFFFFFF;

    for (; symbol != '\0'; symbol = hashableData[++symbolId])
        hash = _mm_crc32_u8 (hash, symbol);

    return hash;
}

```

And you can see dramatic optimization result:

![](data/secondCmp.png)

Then we are profiling again:

![](data/secondProfile.png)

As you can see, List class getter for left/right pointer is using  ~25% of all execution time.
I've simply changed this place in hash table get function:

```c++

    elemP = listP->getPrevOrNext (elemP, List::ListElemSide_t::NEXT_);

```

To this:

```c++

    elemP = elemP->nextP_;

```

And perfomance became a bit better:

![](data/thirdCmp.png)

Then i've profiled my hash table for last time:

![](data/thirdProfile.png)

As you can see, the only function, that can be refactored for better results is the hash table get function. But the only place, that can became better is that while cycle:

```c++

    while (elemP != nullptr)
    {
        if (!strcmp (elemP->listElemData_.hashableData_, data2Seek))
            return elemP->listElemData_;

        elemP = elemP->nextP_;
    }

```

But i've checked asm code, that g++ generates for this while:

![](data/asmFirst.png)

And this asm code is quite optimized. So i decided to stop with optimizations.

### ___Test programm is working ~13 times faster now.___