import matplotlib.pyplot as plt
import numpy as np
import pdb


# Initialize plot
fig = plt.figure()
fig.set_size_inches(6,11.)
fig.subplots_adjust(hspace=0.05, wspace=0.05)

# File Extensions and Sizes
fnames = ["naive_search1_long.txt","simpaccel_search1_long.txt",
			"naive_search1_short.txt","simpaccel_search1_short.txt",
			"naive_search2_long.txt","simpaccel_search2_long.txt",
			"naive_search2_short.txt","simpaccel_search2_short.txt"]

length = np.loadtxt("size_queries.txt",unpack=True)
unique_length = np.unique(length)

# Read the file line by line
fi = 1
for file in fnames:

	print(file)
	if fi ==1:
		ax = fig.add_subplot(4,2,fi)
	else:
		ax = fig.add_subplot(4,2,fi)

	# Read the Data File
	data = np.loadtxt(file,dtype=str)

	# Find The Different Reference Sizes and Lengths 
	str_match = list(filter(lambda x: '>>' in x, data))
	ref_size,ref_length = np.transpose([(ii.split("_")[1],ii.split("_")[2]) for ii in str_match])

	# Use only k = 0
	str_match = np.asarray(str_match)[ref_length == '0']
	ref_size = np.asarray(ref_size)[ref_length == '0']

	# Loop through each str_match
	for ss in range(len(str_match)):

		# Find where the measurements begin
		pos = np.argwhere(data == str_match[ss]).reshape(1)[0]

		this_data = np.double(data[pos+1:pos+10001])

		avgTime = []
		for un in unique_length:
			keep = np.argwhere(length == un).reshape(-1)
			avgTime.append(np.mean(this_data[keep]))

		avgTime = np.asarray(avgTime)

		ax.plot(unique_length,avgTime,marker="o",mew=0.5,ls="--",label=ref_size[ss][:-1]+ref_size[ss][-1].upper()+"B")

	# Define the Title Labeling
	if fi == 1:
		ax.set_title("naive")
		ax.legend(frameon=True,ncol=3,numpoints=1,bbox_to_anchor=(1.75,1.3))
		ax_master = ax

	if fi == 2:
		ax.set_title("simpaccel")


	title = file.split("_")
	ax.text(0.97,0.08,r"%s\_%s\_%s()" % (title[0],title[1],title[2].split(".txt")[0]),transform=ax.transAxes,ha="right",va="center",fontsize=8)


	# Define the ranges	
	ax.set_yscale("log")


	# Set Common Axis per row
	if fi in (1,2):
		ax.set_ylim(1e-6,3e-5)

	if fi in (3,4):
		ax.set_ylim(1e-6,4e-5)

	if fi in (5,6):
		ax.set_ylim(1e-6,3e-3)

	if fi in (7,8):
		ax.set_ylim(1e-6,8e-3)


	# Hide Tick Labels for certain inner subplots
	if fi in (2,4,6):
		ax.set_xticklabels([])
		ax.set_yticklabels([])

	if fi in (1,3,5):
		ax.set_xticklabels([])

	if fi == 8:
		ax.set_yticklabels([])
	
	fi+=1


fig.text(0.05,0.5,r"Elapsed Time (sec)",ha="center",va="center",rotation=90)
fig.text(0.5,0.08,r"Query Length (\# of characters)",ha="center",va="center")

plt.savefig("naive_simpaccel_length_time.png",format="png",dpi=300,bbox_inches="tight")
