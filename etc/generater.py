key  = "w3nl1nglov3jche143"
flag = "flag{just_4_34vm!}"
iv = 143
cmp = []
for i in range(len(flag)):
    result =  (ord(flag[i])+i) ^ ord(key[i]) ^ iv
    iv = result
    cmp.append(result)
print(cmp)
iv = 143
reflag = ''
for i in range(len(cmp)):
    aflag = (cmp[i] ^ iv ^ ord(key[i])) - i
    iv = cmp[i]
    reflag += chr(aflag)
print(reflag)

print("key: ", end='')
for i in key:
    print(ord(i) , end=', ')

print("\nflag: ", end = '')
for i in flag:
    print(ord(i), end = ', ')