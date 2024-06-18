# NASA-Horizons
## DB (SQL)
## Horizons Query Tool (C++)
## Query and DBM ()


```-o, --object``` adds an object
```--longitude``` sets observer longitude
```--detailed``` pulls detailed results from a specific day

    ./horizons 04:04:1391a -o "Sun" -o "Moon" 03:04:1392a 06:07:1393a --longitude N37.5519
            Sun                           | Moon                          |
04:04:1391a 34d12m40s972ms 34d12m40s972ms | 34d12m40s972ms 34d12m40s972ms |
03:04:1392a 34d12m40s972ms 34d12m40s972ms | 34d12m40s972ms 34d12m40s972ms |
06:07:1393a 34d12m40s972ms 34d12m40s972ms | 34d12m40s972ms 34d12m40s972ms |



saved/
  |-1391a-Sun.txt
  |-1392a-Moon.txt

API Calls:
1391a-Moon.txt >> saved
1392a-Sun.txt >> saved
1393a-Sun.txt >> saved
1393a-Moon.txt >> saved