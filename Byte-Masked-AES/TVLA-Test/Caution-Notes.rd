# Caution Notes
It is well known that interpreting the TVLA results is not as trivial as the name \("leakage detection test"\) suggests. For clarity, in the following, we only list a few caution notes relate to this specific implementation. Details about all possible pitfalls can be found in the REASSURE [tutorial on "Understanding Leakage Detection"] (http://reassure.eu/leakage-detection-tutorial/). 


* TVLA results are only qualitative, not quantitative. Although high T-values are mostly a true "leakage sample", there is no guarantee they will lead to easier attacks. Thus, there is no point comparing the maximal T-values among different implementations.

*  By default, TVLA test controls the false-positive rate, rather than the false negative rate. In theory,  there is no solid evidence to prove the "none-leaking" samples are indeed, not leaking.
*  Fix-v.s.-Random strategy only detects plaintext-dependent leakage. In other words, if the key expansion part is not protected \(like this one\), the TVLA test will stay mute for such "leakage". Whether such leakage can be exploited depends on the specific application and attacker model; nonetheless, the 2018 CHES challenge shows that this is definitely [possible in a profiling setting] (https://eprint.iacr.org/2019/094).

*  On the other hand, any plaintext-dependent sample will be detected, even if it does not relate to the secret key \(a.k.a. plaintexts\) or it is difficult to learn the key dependency (a.k.a. ciphertexts). In this specific implementation, we carefully choose the triggered time interval so any plaintext-related leakages are not captured. However, the last AddRoundkey performs the cryptographic operation and "unmasking" at the same time, so there is no doubt this part will show some ciphertext-dependent leakage. We suspect this is what happens between sample 36500 and 37500.

*  A leakage sample does NOT represent one independent "leakage source". In fact, it is quite common to see one leakage source leads to multiple leakage samples on the trace. For instance, an unexpected register/bus transition may introduce some unmasked leakage, which affects many following samples on the TVLA trace. This means when you are addressing the leakages, the number of leakage samples does NOT indicate how many issues are there to fix. Therefore, there is no point comparing the number of samples for different implementations either.

*  Note that side channel information strongly depends on the specific implementation: not only the implementation code but also the specific core and board. Our evaluation here is strictly restricted to this specific setting: for any other architecture or physical setup, it is always worthwhile to confirm the leakage behaviors of the specific device.


*  Last but not least, a good SCA countermeasure should combine both hiding and masking techniques. For this specific implementation, adding some random shuffling to the Sbox/ShiftRow computation will further reduce the signal-to-noise (SNR) ratio.  We tend to avoid hiding techniques in security evaluations,  as they lead to lower SNR, which may in return, hides some exploitable flaws in our masking schemes. However, for end users, hiding technique serves as an effective countermeasure with relatively low cost.
