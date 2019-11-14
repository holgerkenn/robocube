# LaTeX2HTML 98.1p1 release (March 2nd, 1998)
# Associate images original text with physical files.


$key = q/{inline}mboxbullet{inline}MSF=1.6;AAT;/;
$cached_env_img{$key} = q|<IMG
 WIDTH="14" HEIGHT="16" ALIGN="BOTTOM" BORDER="0"
 SRC="img1.gif"
 ALT="$\mbox{$\bullet$}$">|; 

$key = q/{figure}centermakeboxepsfysize=70mmepsfboxgal.epscenter{figure}FSF=1.6;AAT;/;
$cached_env_img{$key} = q|<IMG
 WIDTH="671" HEIGHT="313"
 SRC="img2.gif"
 ALT="\begin{figure}\begin{center}\makebox{\epsfysize=70mm \epsfbox{gal.eps}} \end{center} \end{figure}">|; 

1;

