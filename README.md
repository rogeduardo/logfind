# logfind

This tool is a specialized version fo another tool called grep, but designed for log files on a system.
The idea is that I can type:

    logfind rogerio

And, it will search all the common places that log files are stored, and print out every file that has
the word "rogerio" in it.
The logfind tool have these basic features:
<ol>
    <li>This tool takes any sequence of words and assumes I mean "and" for them. So $logfind rogerio
       smart guy will find all files that have rogerio and smart and guy in them.</li>
    <li>It takes an optional argument of -o if the parameters are ment to be or logic.</li>
    <li>It loads the list of allowed log files from ~/.logfind</li>
    <li>The list of file names can be anything that the glob function allows.</li>
    <li>It output the matching lines as scan.</li>
</ol>

This project was suggested at Learn C the Hard Way: Practical Exercises on the Computational Subjects You Keep Avoiding (Like C)