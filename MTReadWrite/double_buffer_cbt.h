#pragma once
/*******************************
					cbt (changed block tracking)

1. double cbt buffer
	- current_cbt_buffer - bitmap for current job
	- next_cbt_buffer - bitmap for next job



2. Ideally we should monitor changes between 2 snapshots, practically monitor more changes to avoid data loss 
   cbt between pre_snapshot of the prior job and post_snapshot of the current job. 
   

							job 1  ----------------------------------------------------------------------------------------------> job 2
			pre_snapshot-->take_snapshot-->post_snapshot														pre_snapshot-->take_snapshot-->post_snapshot
				|---------------------------------------------------practical cbt for job 2------------------------------------------------------>|
								|-------------------------------------ideal cbt for job 2------------------------------------------>|



3. backup application algorithm

			a. cbt_set_job_satus(failed, out last_status)

			b. cbt_pre_snapshot(last_status)
				{
					if(succeeded == last_status)
					{
						swap(current_cbt_buffer, next_cbt_buffer); //only if last job succeeded, swap current and next buffer to handle the case that the last backup failed
					}
					cbt_clear_bitmap(next_cbt_buffer);
				}
			
			c. take_snapshot

			d. cbt_get_changed_bitmap()
				{
					return current_cbt_buffer;
				}

				note: 
				cbt_get_changed_bitmap() for volume and its snapshot, merge the 2 bitmaps

			e. read_snapshot and backup_data

			f. cbt_set_job_satus(succeeded, out null)


4. driver design
			a. monitor write operations, and write changes to current_cbt_buffer and next_cbt_buffer always
			b. lock and synchronization to operate on cbt buffer
			c. cbt buffer memory usage optimization (one level of indirection, block allocation table, think of VHD/VHDX)
			d. swap(current_cbt_buffer, next_cbt_buffer) need not swap buffer data, it can swap 2 index
***********************************/