Vor "begin{document}":

% visio zu pdf:
% http://officetopdf.codeplex.com/

%% execute scripts
\newcommand{\visioToPDF}[1] {
	\immediate\write18{convert_scripts\string\\visioToPDF.bat #1}
}


Bild einfügen:

\begin{figure}[H]
	\visioToPDF{bilder/allgemein/tracing-schaubild.pdf}
	\includegraphics[width=0.99\textwidth]{bilder/allgemein/tracing-schaubild.pdf}
\end{figure}