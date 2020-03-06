using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using Microsoft.Azure;
using Microsoft.IdentityModel.Clients.ActiveDirectory;
using Microsoft.Azure.Management.ResourceManager;
using Microsoft.Azure.Management.ResourceManager.Models;
using Microsoft.Azure.Management.Storage;
using Microsoft.Azure.Management.Storage.Models;
using Microsoft.Azure.Management.Network;
using Microsoft.Azure.Management.Network.Models;
using Microsoft.Azure.Management.Compute;
using Microsoft.Azure.Management.Compute.Models;
using Microsoft.Rest;
using Microsoft.Rest.Azure;

/***************************************************************************
https://azure.microsoft.com/en-in/documentation/articles/virtual-machines-windows-csharp/
***************************************************************************/

namespace test_sc_azure_sdk
{
    class _000AzureResource
    {
        public static async Task<string> GetAuthorizationHeaderAsync()
        {
            string application_id = "7fdb9056-a9ae-475f-8b21-5e707f77860e";
            string password = "snR/QgCH0YkRqKjWPgBz9vueXhxowlNxAqvPTXyKQAk=";
            string tenant_id = "1a6cd66d-2cc0-4892-b41e-e01c6f0f8350";

            ClientCredential cc = new ClientCredential($"{application_id}", $"{password}");
            var context = new AuthenticationContext($"https://login.windows.net/{tenant_id}");
            var result = await context.AcquireTokenAsync("https://management.azure.com/", cc);

            if (result == null)
            {
                throw new InvalidOperationException("Failed to obtain the JWT token");
            }

            string token = result.AccessToken;

            return token;
        }


        public static async Task GetSubscriptionsAsync(TokenCredentials credential)
        {
            SubscriptionClient client = new SubscriptionClient(credential);
            IPage<Subscription> subscriptions = await client.Subscriptions.ListAsync();
            foreach (var sub in subscriptions)
            {
                Console.WriteLine($"{nameof(sub.DisplayName)}={sub.DisplayName}");
                Console.WriteLine($"{nameof(sub.Id)}={sub.Id}");
                Console.WriteLine($"{nameof(sub.State)}={sub.State}");
                Console.WriteLine($"{nameof(sub.SubscriptionId)}={sub.SubscriptionId}");
                Console.WriteLine();
            }
        }


        public static async Task CreateResourceGroupAsync(
        TokenCredentials credential,
        string groupName,
        string subscriptionId,
        string location
        )
        {
            Console.WriteLine("Creating the resource group...");
            var resourceManagementClient = new ResourceManagementClient(credential);
            resourceManagementClient.SubscriptionId = subscriptionId;
            var resourceGroup = new ResourceGroup
            {
                Location = location
            };
            var rgResult = await resourceManagementClient.ResourceGroups.CreateOrUpdateAsync(groupName, resourceGroup);
            Console.WriteLine(rgResult.Properties.ProvisioningState);

            const string Microsoft_Storage = "Microsoft.Storage";
            var rpResult = await resourceManagementClient.Providers.RegisterAsync(Microsoft_Storage);
            Console.WriteLine($"{Microsoft_Storage} {rpResult.RegistrationState}");

            const string Microsoft_Network = "Microsoft.Network";
            rpResult = await resourceManagementClient.Providers.RegisterAsync(Microsoft_Network);
            Console.WriteLine($"{Microsoft_Network} {rpResult.RegistrationState}");


            const string Microsoft_Compute = "Microsoft.Compute";
            rpResult = await resourceManagementClient.Providers.RegisterAsync(Microsoft_Compute);
            Console.WriteLine($"{Microsoft_Compute} {rpResult.RegistrationState}");
        }


        public static async Task CreateStorageAccountAsync(
        TokenCredentials credential,
        string storageName,
        string groupName,
        string subscriptionId,
        string location
        )
        {
            Console.WriteLine("Creating the storage account...");
            var storageManagementClient = new StorageManagementClient(credential);
            storageManagementClient.SubscriptionId = subscriptionId;
            var saResult = await storageManagementClient.StorageAccounts.CreateAsync(
            groupName,
            storageName,
            new StorageAccountCreateParameters()
            {
    //AccountType = AccountType.StandardLRS,
    Sku = new Microsoft.Azure.Management.Storage.Models.Sku(SkuName.StandardLRS),
                Location = location
            }
            );

            var storKeys = await storageManagementClient.StorageAccounts.ListKeysAsync(groupName, storageName);
            foreach (var key in storKeys.Keys)
            {
                Console.WriteLine($"{nameof(key.KeyName)}={key.KeyName}");
                Console.WriteLine($"{nameof(key.Value)}={key.Value}");
                Console.WriteLine();
            }

            var storAcc = await storageManagementClient.StorageAccounts.GetPropertiesAsync(groupName, storageName);

            Console.WriteLine(saResult.ProvisioningState);
        }

        public static async Task CreatePublicIPAddress(
        TokenCredentials credential,
        string ipName,
        string groupName,
        string subscriptionId,
        string location
        )
        {
            Console.WriteLine("Creating the public ip...");
            var networkManagementClient = new NetworkManagementClient(credential);
            networkManagementClient.SubscriptionId = subscriptionId;
            var ipResult = await networkManagementClient.PublicIPAddresses.CreateOrUpdateAsync(
            groupName,
            ipName,
            new PublicIPAddress
            {
                Location = location,
                PublicIPAllocationMethod = "Dynamic"
            }
            );
            Console.WriteLine(ipResult.ProvisioningState);
        }


        public static async Task CreateNetworkAsync(
        TokenCredentials credential,
        string vnetName,
        string subnetName,
        string nicName,
        string ipName,
        string groupName,
        string subscriptionId,
        string location)
        {
            Console.WriteLine("Creating the virtual network...");
            var networkManagementClient = new NetworkManagementClient(credential);
            networkManagementClient.SubscriptionId = subscriptionId;

            var subnet = new Subnet
            {
                Name = subnetName,
                AddressPrefix = "10.0.0.0/24"
            };

            var address = new AddressSpace
            {
                AddressPrefixes = new List<string> { "10.0.0.0/16" }
            };

            var vnResult = await networkManagementClient.VirtualNetworks.CreateOrUpdateAsync(
            groupName,
            vnetName,
            new VirtualNetwork
            {
                Location = location,
                AddressSpace = address,
                Subnets = new List<Subnet> { subnet }
            }
            );

            Console.WriteLine(vnResult.ProvisioningState);

            var subnetResponse = await networkManagementClient.Subnets.GetAsync(
            groupName,
            vnetName,
            subnetName
            );

            var pubipResponse = await networkManagementClient.PublicIPAddresses.GetAsync(groupName, ipName);

            Console.WriteLine("Updating the network with the nic...");
            var nicResult = await networkManagementClient.NetworkInterfaces.CreateOrUpdateAsync(
            groupName,
            nicName,
            new NetworkInterface
            {
                Location = location,
                IpConfigurations = new List<NetworkInterfaceIPConfiguration>
            {
new NetworkInterfaceIPConfiguration
{
Name = "nicConfig1",
PublicIPAddress = pubipResponse,
Subnet = subnetResponse
}
            }
            }
            );
            Console.WriteLine(vnResult.ProvisioningState);
        }

        public static async Task CreateAvailabilitySetAsync(
        TokenCredentials credential,
        string avsetName,
        string groupName,
        string subscriptionId,
        string location)
        {
            Console.WriteLine("Creating the availability set...");
            var computeManagementClient = new ComputeManagementClient(credential);
            computeManagementClient.SubscriptionId = subscriptionId;
            var avResult = await computeManagementClient.AvailabilitySets.CreateOrUpdateAsync(
            groupName,
            avsetName,
            new AvailabilitySet()
            {
                Location = location
            }
            );
        }


        public static async Task CreateVirtualMachineAsync(
        TokenCredentials credential,
        string vmName,
        string groupName,
        string nicName,
        string avsetName,
        string storageName,
        string adminName,
        string adminPassword,
        string subscriptionId,
        string location)
        {
            var networkManagementClient = new NetworkManagementClient(credential);
            networkManagementClient.SubscriptionId = subscriptionId;
            var nic = await networkManagementClient.NetworkInterfaces.GetAsync(groupName, nicName);

            var computeManagementClient = new ComputeManagementClient(credential);
            computeManagementClient.SubscriptionId = subscriptionId;
            var avSet = await computeManagementClient.AvailabilitySets.GetAsync(groupName, avsetName);


            try
            {
                var sampleVM = await computeManagementClient.VirtualMachines.GetAsync("hbbu-res-group", "hbbu-vm");
                Console.WriteLine($"{nameof(sampleVM.Name)}={sampleVM.Name}");
            }
            catch (System.Exception ex)
            {
                Console.WriteLine($"{ex}");
            }

            var vmSizes = await computeManagementClient.VirtualMachineSizes.ListAsync(location);
            foreach (var vmSize in vmSizes)
            {
                Console.WriteLine($"{nameof(vmSize.Name)}={vmSize.Name}");
                Console.WriteLine($"{nameof(vmSize.MaxDataDiskCount)}={vmSize.MaxDataDiskCount}");
                Console.WriteLine($"{nameof(vmSize.MemoryInMB)}={vmSize.MemoryInMB}");
                Console.WriteLine($"{nameof(vmSize.NumberOfCores)}={vmSize.NumberOfCores}");
                Console.WriteLine($"{nameof(vmSize.OsDiskSizeInMB)}={vmSize.OsDiskSizeInMB}");
                Console.WriteLine($"{nameof(vmSize.ResourceDiskSizeInMB)}={vmSize.ResourceDiskSizeInMB}");
                Console.WriteLine();
            }



            Console.WriteLine("Creating the virtual machine...");

            bool bCreateNewDataDisk = true;
            bool bAttcheExisting = true;
            var vm = await computeManagementClient.VirtualMachines.CreateOrUpdateAsync(
            groupName,
            vmName,
            new VirtualMachine
            {
                Location = location,
                AvailabilitySet = new Microsoft.Azure.Management.Compute.Models.SubResource
                {
                    Id = avSet.Id
                },
                HardwareProfile = new HardwareProfile
                {
                    //VmSize = "Standard_A0"
                    VmSize = "Standard_DS1_V2"
                },
                OsProfile = new OSProfile
                {
                    AdminUsername = adminName,
                    AdminPassword = adminPassword,
                    ComputerName = vmName,
                    WindowsConfiguration = new WindowsConfiguration
                    {
                        ProvisionVMAgent = true
                    }
                },
                NetworkProfile = new NetworkProfile
                {
                    NetworkInterfaces = new List<NetworkInterfaceReference>
                    {
                        new NetworkInterfaceReference { Id = nic.Id }
                    }
                },

                StorageProfile = new StorageProfile
                {
                    ImageReference = new ImageReference
                    {
                        Publisher = "MicrosoftWindowsServer",
                        Offer = "WindowsServer",
                        Sku = "2012-R2-Datacenter",
                        Version = "latest"
                    },
                    OsDisk = new OSDisk
                    {
                        Name = "mytestod1",
                        CreateOption = DiskCreateOptionTypes.FromImage,
                        Vhd = new VirtualHardDisk
                        {
                            Uri = "https://" + storageName + ".blob.core.windows.net/vhds/mytestod1.vhd"
                        }
                    },
                    DataDisks = bCreateNewDataDisk? new List<DataDisk>()
                    {
                        new DataDisk()
                        {
                            Caching = CachingTypes.ReadOnly,
                            CreateOption = bAttcheExisting? DiskCreateOptionTypes.Attach : DiskCreateOptionTypes.Empty,
                            DiskSizeGB = 1023,
                            Lun = 0,
                            Name = "sssDataDisk0",
                            Vhd = new VirtualHardDisk()
                            {
                                Uri = "https://" + storageName + ".blob.core.windows.net/vhds/sssDataDisk0.vhd"
                            }
                        },


                        new DataDisk()
                        {
                            Caching = CachingTypes.ReadOnly,
                            CreateOption = bAttcheExisting? DiskCreateOptionTypes.Attach : DiskCreateOptionTypes.Empty,
                            DiskSizeGB = 1023,
                            Lun = 1,
                            Name = "sssDataDisk1",
                            Vhd = new VirtualHardDisk()
                            {
                                Uri = "https://" + storageName + ".blob.core.windows.net/vhds/sssDataDisk1.vhd"
                            }
                        }
                    } : 
                    new List<DataDisk>()
                }
            }
            );
            Console.WriteLine(vm.ProvisioningState);
        }


        public static async Task PowerOnOffVirtualMachineAsync(
        TokenCredentials credential,
        string vmName,
        string groupName,
        string subscriptionId)

        {
            ComputeManagementClient client = new ComputeManagementClient(credential);
            client.SubscriptionId = subscriptionId;

            VirtualMachine vm = await client.VirtualMachines.GetAsync(groupName, vmName);
            Console.WriteLine("Power Off VM...");
            await client.VirtualMachines.PowerOffAsync(groupName, vmName);
            Console.WriteLine("Start VM...");
            await client.VirtualMachines.StartAsync(groupName, vmName);
            Console.WriteLine("Power Off VM...");
            await client.VirtualMachines.PowerOffAsync(groupName, vmName);
        }           
            



        public static async Task DeleteResourceGroupAsync(
        TokenCredentials credential,
        string groupName,
        string subscriptionId)
        {
            Console.WriteLine("Deleting resource group...");
            var resourceGroupClient = new ResourceManagementClient(credential);
            resourceGroupClient.SubscriptionId = subscriptionId;
            await resourceGroupClient.ResourceGroups.DeleteAsync(groupName);
        }

        public static async Task TestAzureOperationsAsync()
        {
            try
            {

                string token = await GetAuthorizationHeaderAsync();
                var credential = new TokenCredentials(token);


                await GetSubscriptionsAsync(credential);

                var groupName = "sssgroupname";
                var ipName = "sssipname";
                var avSetName = "sssavsetname";
                var nicName = "sssnicname";
                var storageName = "sssstoragename";
                var vmName = "sssvmname";
                var vnetName = "sssvnetname";
                var subnetName = "ssssubnetname";
                var adminName = "sss";
                var adminPassword = "sssC@1234567";
                var location = "eastus";
                var subscriptionId = "7315bf64-0ed9-46d8-b4e6-1a6082194d22";


                await CreateResourceGroupAsync(
                credential,//TokenCredentials credential,
                groupName,//string groupName,
                subscriptionId,//string subscriptionId,
                location//string location
                );
                //Console.ReadLine();


                await CreateStorageAccountAsync(
                credential,
                storageName,
                groupName,
                subscriptionId,
                location);
                //Console.ReadLine();


                await CreatePublicIPAddress(
                credential,
                ipName,
                groupName,
                subscriptionId,
                location
                );


                await CreateNetworkAsync(
                credential,
                vnetName,
                subnetName,
                nicName,
                ipName,
                groupName,
                subscriptionId,
                location);


                await CreateAvailabilitySetAsync(
                credential,
                avSetName,
                groupName,
                subscriptionId,
                location);
                //Console.ReadLine();


                await CreateVirtualMachineAsync(
                credential,
                vmName,
                groupName,
                nicName,
                avSetName,
                storageName,
                adminName,
                adminPassword,
                subscriptionId,
                location);
                //Console.ReadLine();

                await PowerOnOffVirtualMachineAsync(
                credential,//TokenCredentials credential,
                vmName,//string vmName,
                groupName,//string groupName,
                subscriptionId//string subscriptionId
                );

                bool toDeleteResource = false;
                if(toDeleteResource)
                {
                    await DeleteResourceGroupAsync(
                            credential,
                            groupName,
                            subscriptionId);
                }
                
                //Console.ReadLine();
            }
            catch (System.Exception ex)
            {
                Console.WriteLine($"{ex.Message}");
                Console.WriteLine($"{ex}");
                throw;
            }

        }
    }


    static class _000AzureResourceTest
    {
        public static void TestCases(int testCase)
        {
            switch (testCase)
            {
                case 1:
                    {
                        Task task = _000AzureResource.TestAzureOperationsAsync();
                        task.Wait();
                    }
                    break;
                case 0:
                    {
                        Task<string> task = _000AzureResource.GetAuthorizationHeaderAsync();
                        task.Wait();
                        string token = task.Result;
                        Console.WriteLine($"{nameof(token)}={token}");
                    }
                    break;
                default:
                    {
                        throw new InvalidOperationException("invalid test case number");
                    }


            }
        }
    }
}
