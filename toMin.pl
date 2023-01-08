while(<>) {
	$_ =~ s/BIT\((\w*)(, \d)?\)/"BIT(".lc($1)."$2)"/ge;
	$_ =~ s/REGISTER\((\w*)/"REGISTER(".lc($1)/ge;
	print;
}
