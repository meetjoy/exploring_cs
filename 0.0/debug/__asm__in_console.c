   1d2cd:	89 c3                	mov    %eax,%ebx
   1d2cf:	0f b6 4c 24 0b       	movzbl 0xb(%esp),%ecx
   1d2d4:	8b 54 24 0c          	mov    0xc(%esp),%edx
   1d2d8:	89 d0                	mov    %edx,%eax
   1d2da:	c1 e0 03             	shl    $0x3,%eax
   1d2dd:	01 d0                	add    %edx,%eax
   1d2df:	c1 e0 04             	shl    $0x4,%eax
   1d2e2:	8d 90 62 fe 03 00    	lea    0x3fe62(%eax),%edx
   1d2e8:	89 c8                	mov    %ecx,%eax
   1d2ea:	8a 22                	mov    (%edx),%ah
   1d2ec:	66 89 03             	mov    %ax,(%ebx)   #addreesing wrong
   1d2ef:	8b 54 24 0c          	mov    0xc(%esp),%edx
   1d2f3:	89 d0                	mov    %edx,%eax
   1d2f5:	c1 e0 03             	shl    $0x3,%eax
   1d2f8:	01 d0                	add    %edx,%eax
   1d2fa:	c1 e0 04             	shl    $0x4,%eax
   1d2fd:	05 80 fe 03 00       	add    $0x3fe80,%eax
   1d302:	8b 00                	mov    (%eax),%eax
