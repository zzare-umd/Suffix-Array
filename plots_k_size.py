import matplotlib.pyplot as plt
import numpy as np
import pdb


# Initialize plot
fig = plt.figure()
fig.set_size_inches(5,5)
ax = fig.add_subplot(111)


# File Extensions and Sizes
fnames = ["11KB","20KB","1.6MB","4.6MB","49MB"]

# Load in the Data (format=k,10k,20k,2M,4.4M,48M)
data = np.loadtxt("file_sizes.txt",unpack=True)

# Loop over each file name extension
for dd in range(len(fnames)):

	# plot the data
	ax.plot(data[0,:],data[dd+1,:],marker="o",mec="black",mew=0.5,ls="--",label=fnames[dd])


# Define Legend
ax.legend(frameon=False,ncol=3,numpoints=1,loc="upper left")

# Define Labels
ax.set_title(r"build\_sa() output size")
ax.set_xlabel(r"$k$ (prefix size)")
ax.set_ylabel(r"Size (MB)")


# Define Axis Limits and Scaling
ax.set_yscale("log")
ax.set_ylim(3e-2,3e3)

# Save Plot
plt.savefig("elapsed_size.png",format="png",dpi=300,bbox_inches="tight")

