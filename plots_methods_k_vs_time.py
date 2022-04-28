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


	# Loop through each str_match
	avgTime = []
	for ss in range(len(str_match)):

		# Find where the measurements begin
		pos = np.argwhere(data == str_match[ss]).reshape(1)[0]

		avgTime.append(np.mean(np.double(data[pos+1:pos+10000])))

	avgTime = np.asarray(avgTime)
	unique_ref = ref_size[np.sort(np.unique(ref_size, return_index=True)[1])]

	for rr in range(len(unique_ref)):
		keep = ref_size == unique_ref[rr]

		# Still running the code so this last one is an incomplete output
		if unique_ref[rr] == "1.6M":
			ax.plot(np.double(ref_length[keep][:-1]),avgTime[keep][:-1],marker="o",mew=0.5,ls="--",label=unique_ref[rr][:-1]+unique_ref[rr][-1].upper()+"B")
		else:
			# Plot the data
			ax.plot(np.double(ref_length[keep]),avgTime[keep],marker="o",mew=0.5,ls="--",label=unique_ref[rr][:-1]+unique_ref[rr][-1].upper()+"B")

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
	if fi > 1:
		ax.set_xlim(ax_master.get_xlim())
		ax.set_ylim(ax_master.get_ylim())

	if fi in (2,4,6):
		ax.set_xticklabels([])
		ax.set_yticklabels([])

	if fi in (1,3,5):
		ax.set_xticklabels([])

	if fi == 8:
		ax.set_yticklabels([])

	fi+=1


fig.text(0.05,0.5,r"Elapsed Time (sec)",ha="center",va="center",rotation=90)
fig.text(0.5,0.08,r"$k$ (prefix size)",ha="center",va="center")

plt.savefig("naive_simpaccel_elapsed_time.png",format="png",dpi=300,bbox_inches="tight")

exit()

title = file.split("_")
ax.set_title(r"%s\_%s\_%s()" % (title[0],title[1],title[2].split(".txt")[0]))
ax.set_xlabel(r"$k$ (prefix size)")
ax.set_ylabel(r"Elapsed Time (sec)")
ax.set_yscale("log")
plt.show()

pdb.set_trace()


# Loop over each file name extension
for dd in range(len(fnames)):

	kval,time = np.loadtxt("elapsed_time%s.txt" % fnames[dd],unpack=True)

	# plot the data
	ax.plot(kval,time,marker="o",mec="black",mew=0.5,ls="--",label=labels[dd])


# Define Legend
ax.legend(frameon=False,ncol=3,numpoints=1,loc="upper right")

# Define Labels
ax.set_title(r"build\_sa() time")
ax.set_xlabel(r"$k$ (prefix size)")
ax.set_ylabel(r"elapsed time (sec)")


# Define Axis Limits and Scaling
ax.set_yscale("log")
ax.set_ylim(7e-4,3e2)


# Save Plot
plt.savefig("elapsed_time.png",format="png",dpi=300,bbox_inches="tight")

