using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using System.Security.AccessControl;
using System.Security.Principal;

namespace ModifyFileSec
{
  public  class FileSystemSec
    {
        public static void API(string name, string owner, string user, FileSystemRights right = FileSystemRights.FullControl, AccessControlType access = AccessControlType.Allow)
        {
            FileAttributes fileAttr = File.GetAttributes(name);
            if (fileAttr.HasFlag(FileAttributes.Directory))
            {
                ModifyDirOwnerRec(name, owner);
                ModifyDirSec(name, user, right, access);
            }
            else
            {
                ModifyFileOwner(name, owner);
                ModifyFileSec(name, user, right, access);
            }
        }

        public static void ModifyDirOwnerRec(string name, string owner)
        {
            if(name == "." || name == "..")
            {
                return;
            }

            ModifyDirOwner(name, owner);
            var di = new DirectoryInfo(name);
            foreach(var entry in di.EnumerateFileSystemInfos())
            {
                if(entry.Attributes.HasFlag(FileAttributes.Directory))
                {
                    ModifyDirOwnerRec(entry.FullName, owner);
                }
                else
                {
                    ModifyFileOwner(name, owner);
                }
            }
        }

        
        public static void ModifyFileOwner(string file, string owner)
        {
            try
            {
                Console.WriteLine("~~~~~~~~~~~~~ModifyFileOwner~~~~~~~~~~~~~~~~~~~~");
                FileInfo fi = new FileInfo(file);

                /////////////////////////////////////////////////////////////
                FileSecurity fssec = fi.GetAccessControl();
                SecurityIdentifier sid = fssec.GetOwner(typeof(SecurityIdentifier)) as SecurityIdentifier;
                NTAccount account = sid.Translate(typeof(NTAccount)) as NTAccount;
                Console.WriteLine($"Original owner sid is '{sid}', user account is '{account}' of file '{file}'");


                /////////////////////////////////////////////////////////////
                NTAccount current = WindowsIdentity.GetCurrent().User.Translate(typeof(NTAccount)) as NTAccount;
                NTAccount newOwner = string.IsNullOrEmpty(owner) ? current : new NTAccount(owner);
                SecurityIdentifier newSidOwner = newOwner.Translate(typeof(SecurityIdentifier)) as SecurityIdentifier;
                Console.WriteLine($"Trying to set new owner sid is '{newSidOwner}', user account is '{newOwner}' of file '{file}'");


                /////////////////////////////////////////////////////////////
                fssec.SetOwner(newOwner);
                fi.SetAccessControl(fssec);


                ///////////////////////////////////////////////////////////
                fssec = fi.GetAccessControl();
                sid = fssec.GetOwner(typeof(SecurityIdentifier)) as SecurityIdentifier;
                account = sid.Translate(typeof(NTAccount)) as NTAccount;
                Console.WriteLine($"New owner sid is '{sid}', user account is '{account}' of file '{file}'");

            }
            catch (Exception ex)
            {
                Console.WriteLine($"failed to ModifyFileOwner for {file} with error {ex.Message} ");
                //throw;
            }
            

        }

        public static void ModifyFileSec(string file, string user, FileSystemRights rights, AccessControlType access)
        {
            try
            {
                Console.WriteLine("~~~~~~~~~~~~~ModifyFileSec~~~~~~~~~~~~~~~~~~~~");
                FileInfo fi = new FileInfo(file);

                /////////////////////////////////////////////////////////////
                FileSecurity fssec = fi.GetAccessControl();

                NTAccount current = WindowsIdentity.GetCurrent().User.Translate(typeof(NTAccount)) as NTAccount;
                NTAccount newUser = string.IsNullOrEmpty(user) ? current : new NTAccount(user);
                FileSystemAccessRule rule = new FileSystemAccessRule(newUser, rights, access);
                fssec.AddAccessRule(rule);
                fi.SetAccessControl(fssec);
                Console.WriteLine($"set access rule '{rule.IdentityReference.Translate(typeof(NTAccount))}, {rule.FileSystemRights}, {rule.AccessControlType}' for {file}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"failed to ModifyFileSec for {file} with error {ex.Message} ");
                //throw;
            }
            
        }


        public static void ModifyDirOwner(string dir, string owner)
        {
            try
            {
                Console.WriteLine("*****************ModifyDirOwner*****************************");
                DirectoryInfo di = new DirectoryInfo(dir);

                /////////////////////////////////////////////////////////////
                DirectorySecurity dirsec = di.GetAccessControl();
                SecurityIdentifier sid = dirsec.GetOwner(typeof(SecurityIdentifier)) as SecurityIdentifier;
                NTAccount account = sid.Translate(typeof(NTAccount)) as NTAccount;
                Console.WriteLine($"Original owner sid is '{sid}', user account is '{account}' of directory '{dir}'");


                /////////////////////////////////////////////////////////////
                NTAccount current = WindowsIdentity.GetCurrent().User.Translate(typeof(NTAccount)) as NTAccount;
                NTAccount newOwner = string.IsNullOrEmpty(owner) ? current : new NTAccount(owner);
                SecurityIdentifier newSidOwner = newOwner.Translate(typeof(SecurityIdentifier)) as SecurityIdentifier;
                Console.WriteLine($"Trying to set new owner sid is '{newSidOwner}', user account is '{newOwner}' of directory '{dir}'");


                /////////////////////////////////////////////////////////////
                dirsec.SetOwner(newOwner);
                di.SetAccessControl(dirsec);


                ///////////////////////////////////////////////////////////
                dirsec = di.GetAccessControl();
                sid = dirsec.GetOwner(typeof(SecurityIdentifier)) as SecurityIdentifier;
                account = sid.Translate(typeof(NTAccount)) as NTAccount;
                Console.WriteLine($"New owner sid is '{sid}', user account is '{account}' of directory '{dir}'");

            }
            catch (Exception ex)
            {
                Console.WriteLine($"failed to ModifyDirOwner for {dir} with error {ex.Message} ");
                //throw;
            }
            

        }


        public static void ModifyDirSec(string dir , string user, FileSystemRights rights, AccessControlType access)
        {
            try
            {
                Console.WriteLine("*****************ModifyDirSec*****************************");
                DirectoryInfo di = new DirectoryInfo(dir);

                DirectorySecurity dirsec = di.GetAccessControl();

                NTAccount current = WindowsIdentity.GetCurrent().User.Translate(typeof(NTAccount)) as NTAccount;

                dirsec = di.GetAccessControl();

                ///////////////////////////////////////////////////////////
                NTAccount newUser = string.IsNullOrEmpty(user) ? current : new NTAccount(user);
                FileSystemAccessRule rule = new FileSystemAccessRule(newUser, rights, InheritanceFlags.ContainerInherit | InheritanceFlags.ObjectInherit, PropagationFlags.InheritOnly, access);
                dirsec.AddAccessRule(rule);
                di.SetAccessControl(dirsec);
                Console.WriteLine($"set access rule '{rule.IdentityReference.Translate(typeof(NTAccount))}, {rule.FileSystemRights}, {rule.AccessControlType}' for {dir}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"failed to ModifyDirSec for {dir} with error {ex.Message} ");
                // throw;
            }
            
        }
    }
}
