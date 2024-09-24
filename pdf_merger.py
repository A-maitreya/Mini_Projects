import PyPDF2

n=int(input("Enter the number of pdfs you want to merge: "))
pdfs = []
for i in range(n):
    pdfs.append(input("Enter the pdf file location: "))
merger=PyPDF2.PdfMerger()
for filename in pdfs:
    pdfs = open(filename, 'rb')
    pdfReader = PyPDF2.PdfReader(pdfs)
    merger.append(pdfReader)
pdfs.close()
merger.write("Merged.pdf")